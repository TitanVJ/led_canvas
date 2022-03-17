#include "micInput.h"
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "fileIO.h"

#define A2D_FILE_VOLTAGE1 "/sys/bus/iio/devices/iio:device0/in_voltage1_raw"

static bool stopThread = false;
static pthread_t threadID;

static void* micThread() {
    while (!stopThread) {
        int rawValue = readFromFile(A2D_FILE_VOLTAGE1);
        //printf("Mic reading: %d\n", rawValue);
    }
    return NULL;
}

void Mic_startListening(void) {
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
}