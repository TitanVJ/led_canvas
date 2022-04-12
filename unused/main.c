#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "joyStick.h"


static bool isProgramRunning = true;


int main(int argc, char *argv[])
{
    
    printf("Starting to Play...\n");

    
    joyStick_start_thread();
    
    
    while(isProgramRunning);
    
    
    joyStick_stop_thread();
    
    printf("Done!\n");

    return 0;
}