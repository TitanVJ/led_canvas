#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "fileIO.h"

void sleep_micro(long msec) {
	struct timespec sleep_time;
	sleep_time.tv_sec = (msec / 1000);
	sleep_time.tv_nsec = (msec % 1000) * 1000000;
	nanosleep(&sleep_time, NULL);
}

int readFromFile(char* filename) {
	FILE* pFile = fopen(filename, "r");
	if (pFile == NULL) {
		printf("ERROR: Unable to open file (%s) for read\n", filename);
	}

	// read string (line)
	const int MAX_LENGTH = 1024;
	char buff[MAX_LENGTH];
	fgets(buff, MAX_LENGTH, pFile);

	// close
	fclose(pFile);

	return atoi(buff);
}

void writeToFile(char* filename, int val) {
	FILE* pFile = fopen(filename, "w");
	if (pFile == NULL) {
		printf("ERROR: Unable to open write file (%s).\n", filename);
		exit(1);
	}	
	// Write to data to the file using fprintf();
	fprintf(pFile, "%d", val);
	
	// Close the file
	fclose(pFile);
	sleep_micro(300);
}
