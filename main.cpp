#include <iostream>
#include "controller.h"
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <linux/joystick.h>

//NOTE: does not use the controller class - 
//I do not know the buttons and their names yet
//When the inputs are tested, you can use that to get the proper enum classs

int main(int argc, char* argv[]) {
    const char* device;
    int js;
    struct js_event event;
    struct axis_state axes[3] = { 0 };
    int axis;

    //must change "device" or argument to location of controller
    if (argc > 1)
        device = argv[1];
    else
        //temp: change when running!
        device = "/dev/input/js0";

    js = open(device, O_RDONLY);

    if (js == -1)
        perror("Could not open joystick");

    // This loop will exit if the controller is unplugged. 
    while (read_event(js, &event) == 0)
    {
        switch (event.type)
        {
        case JS_EVENT_BUTTON:
            printf("Button %u %s\n", event.number, event.value ? "pressed" : "released");
            break;
        case JS_EVENT_AXIS:
            axis = get_axis_state(&event, axes);
            if (axis < 3)
                printf("Axis %zu at (%6d, %6d)\n", axis, axes[axis].x, axes[axis].y);
            break;
        default:
            /* Ignore init events. */
            break;
        }

        fflush(stdout);
    }

    close(js);
    return 0;
}