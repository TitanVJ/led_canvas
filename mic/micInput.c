#include "micInput.h"
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

#include "fileIO.h"

#define A2D_FILE_VOLTAGE1 "/sys/bus/iio/devices/iio:device0/in_voltage3_raw"
#define MAX_A2D_READING 4095
#define A2D_VOLTAGE_REF_V 1.8
#define HISTORY_SIZE 500
#define RECENT_SIZE 30

static bool stopThread = false;
static pthread_t threadID;

static int* history = NULL;
static int* recent = NULL;
static int history_head = 0;
static int history_tail = 0;
static int recent_head = 0;
static int recent_tail = 0;


#define WEIGHTING .999
#define THRESHOLD 500
#define HYSTERESIS 100
static double histAvg = 0;
static double recentAvg = 0;
static bool recentFull = false;
static bool aboveThreshold = false;

// circular buffer implementation taken from Brian's as2 solution
static void addReadingToHistory(int* buffer, int* head, int* tail, int bufSize, float reading) {
    buffer[*tail] = reading;
    *tail = (*tail + 1) % bufSize;

    // If full, move head forward.
    if (*tail == *head) {
        recentFull = true;
        *head = (*head + 1) % HISTORY_SIZE;
    }
}

static float average(int* buffer, int bufSize) {
    if (!recentFull) {
        bufSize = recent_tail;
    }

    float sum = 0;
    for (int i=0; i<bufSize; i++) {
        sum += buffer[i];
    }

    return sum / (float)bufSize;
}

static void checkLoudNoise() {
    // loud sound detection heavily inspired from
    // https://opencoursehub.cs.sfu.ca/bfraser/grav-cms/cmpt433/links/files/2018-student-howtos/ElectretMicToDetectClappingOnBBG.pdf
    // and light sampler assignment
    int difference = recentAvg - histAvg;
    if (difference > THRESHOLD) {
        printf("****LOUD SOUND DETECTED****.\n");
        // function call to get game input
        aboveThreshold = true;
        
    } else {
        if (difference < (THRESHOLD - HYSTERESIS)) {
            aboveThreshold = false;
        }
    }
}

static void* micThread() {
    struct timespec sleep_time;
	sleep_time.tv_sec = (100 / 1000000);
	sleep_time.tv_nsec = (100 % 1000000) * 1000;

    while (!stopThread) {
        int rawValue = readFromFile(A2D_FILE_VOLTAGE1);

        addReadingToHistory(history, &history_head, &history_tail, HISTORY_SIZE, rawValue);
        addReadingToHistory(recent, &recent_head, &recent_tail, RECENT_SIZE, rawValue);
        
        histAvg = WEIGHTING * rawValue + (1 - WEIGHTING) * histAvg;
        recentAvg = average(recent, RECENT_SIZE);
        printf("Mic reading: %d\n", rawValue);

        checkLoudNoise();
        nanosleep(&sleep_time, NULL);
    }
    return NULL;
}

void Mic_startListening(void) {
    history = malloc(sizeof(*history) * HISTORY_SIZE);
    recent = malloc(sizeof(*recent) * RECENT_SIZE);
    memset(history, 0, HISTORY_SIZE * sizeof(*history));
    memset(recent, 0, RECENT_SIZE * sizeof(*recent));

    if (pthread_create(
    &threadID,
    NULL,
    micThread,
    NULL)) {
        printf("ERROR: Failed to create Mic listener thread.\n");
    }
}

void Mic_stopListening(void) {
    stopThread = true;
    pthread_join(threadID, NULL);

    free(history);
    free(recent);
}