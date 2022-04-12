/*
 * Some of the code was taken from my own solution to assignment 1
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include "UDP.h"

#define EXPORT_FILE    "/sys/class/gpio/export"

#define JOYSTICK_UP "26"
#define JOYSTICK_DOWN "46"
#define JOYSTICK_LEFT "65"
#define JOYSTICK_RIGHT "47"
#define JOYSTICK_CENTER "27"

#define JOYSTICK_UP_DIRECTION "/sys/class/gpio/gpio26/direction"
#define JOYSTICK_DOWN_DIRECTION "/sys/class/gpio/gpio46/direction"
#define JOYSTICK_LEFT_DIRECTION "/sys/class/gpio/gpio65/direction"
#define JOYSTICK_RIGHT_DIRECTION "/sys/class/gpio/gpio47/direction"
#define JOYSTICK_CENTER_DIRECTION "/sys/class/gpio/gpio27/direction"

#define JOYSTICK_UP_EDGE "/sys/class/gpio/gpio26/edge"
#define JOYSTICK_DOWN_EDGE "/sys/class/gpio/gpio46/edge"
#define JOYSTICK_LEFT_EDGE "/sys/class/gpio/gpio65/edge"
#define JOYSTICK_RIGHT_EDGE "/sys/class/gpio/gpio47/edge"
#define JOYSTICK_CENTER_EDGE "/sys/class/gpio/gpio27/edge"

#define JOYSTICK_UP_VALUE "/sys/class/gpio/gpio26/value"
#define JOYSTICK_DOWN_VALUE "/sys/class/gpio/gpio46/value"
#define JOYSTICK_LEFT_VALUE "/sys/class/gpio/gpio65/value"
#define JOYSTICK_RIGHT_VALUE "/sys/class/gpio/gpio47/value"
#define JOYSTICK_CENTER_VALUE "/sys/class/gpio/gpio27/value"

#define JOYSTICK_UP_ACTIVELOW "/sys/class/gpio/gpio26/active_low"
#define JOYSTICK_DOWN_ACTIVELOW "/sys/class/gpio/gpio46/active_low"
#define JOYSTICK_LEFT_ACTIVELOW "/sys/class/gpio/gpio65/active_low"
#define JOYSTICK_RIGHT_ACTIVELOW "/sys/class/gpio/gpio47/active_low"
#define JOYSTICK_CENTER_ACTIVELOW "/sys/class/gpio/gpio27/active_low"


static pthread_t id1;
static bool isRunning = true;

static void sleep_joyStick(long sec, long nanosec)
{
    long seconds = sec;
    long nanoseconds = nanosec;
    struct timespec reqDelay = {seconds, nanoseconds};
    nanosleep(&reqDelay, (struct timespec *) NULL);
    
}

static void openFileToWrite(char * filePath, char * value)
{
	FILE * file = fopen(filePath, "w");
	
	if(file == NULL){
		printf("ERROR OPENING %s. \n", filePath);
		exit (1);
	}

	int charWritten = fprintf(file, "%s", value);

	if(charWritten <= 0){
		printf("ERROR WRITING DATA \n");
		exit(1);
	}
	fclose(file);
	
}

static int openFileToRead(char * filePath)
{
	FILE * file = fopen(filePath, "r");
	
	if(file == NULL){
		printf("ERROR OPENING %s. \n", filePath);
		exit (1);
	}
	int joyStickReading = 0;
	int itemRead = fscanf(file, "%d", &joyStickReading);
	if (itemRead <= 0) {
		printf("ERROR: Unable to read values from voltage input file.\n");
		exit(-1);
	}
	fclose(file);

	return joyStickReading;
}

static void joyStick_int(void)
{
	openFileToWrite(EXPORT_FILE, JOYSTICK_UP);
	openFileToWrite(EXPORT_FILE, JOYSTICK_DOWN);
	openFileToWrite(EXPORT_FILE, JOYSTICK_LEFT);
	openFileToWrite(EXPORT_FILE, JOYSTICK_RIGHT);
	openFileToWrite(EXPORT_FILE, JOYSTICK_CENTER);
	
	sleep_joyStick(0,900000000);  

	openFileToWrite(JOYSTICK_UP_DIRECTION,"in");
	openFileToWrite(JOYSTICK_DOWN_DIRECTION,"in");
	openFileToWrite(JOYSTICK_LEFT_DIRECTION,"in");
	openFileToWrite(JOYSTICK_RIGHT_DIRECTION,"in");
	openFileToWrite(JOYSTICK_CENTER_DIRECTION,"in");

	openFileToWrite(JOYSTICK_UP_EDGE,"rising");
	openFileToWrite(JOYSTICK_DOWN_EDGE,"rising");
	openFileToWrite(JOYSTICK_LEFT_EDGE,"rising");
	openFileToWrite(JOYSTICK_RIGHT_EDGE,"rising");
	openFileToWrite(JOYSTICK_CENTER_EDGE,"rising");

	openFileToWrite(JOYSTICK_UP_ACTIVELOW,"1");
	openFileToWrite(JOYSTICK_DOWN_ACTIVELOW,"1");
	openFileToWrite(JOYSTICK_LEFT_ACTIVELOW,"1");
	openFileToWrite(JOYSTICK_RIGHT_ACTIVELOW,"1");
	openFileToWrite(JOYSTICK_CENTER_ACTIVELOW,"1");

}

static void * joyStick_input(void * arg)
{
	joyStick_int();
	while(isRunning){
		sleep_joyStick(0,500000000); //100ms
		if(openFileToRead(JOYSTICK_UP_VALUE) == 1){
			listenToUPD();
		}
	}
	return NULL;
}

void joyStick_start_thread(void)
{
	pthread_create(&id1, NULL, &joyStick_input, NULL);
}

void joyStick_stop_thread(void)
{
    isRunning = false;
    pthread_join(id1, NULL);
}