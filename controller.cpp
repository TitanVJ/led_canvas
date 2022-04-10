// Adapted from and heavily inspired by SDL_GameControllerAxis & SDL_GameControlerButton
// Inspired by https://www.youtube.com/watch?v=vbBEG6SvryA&list=WL&index=24&t=488s
// Inspired by https://gist.github.com/jasonwhite/c5b2048c15993d285130


#include <iostream>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <linux/joystick.h>
#include "controller.h"

Joystick::Joystick() 
{
    deviceRoute = "/dev/input/js0";
    js = open(device, O_RDONLY);
    if (js == -1)
        perror("Could not open joystick");
}


Joystick::Joystick(char * location)
{
    deviceRoute = location;
    js = open(device, O_RDONLY);
    if (js == -1)
        perror("Could not open joystick");
}


int Joystick::ReadEvent()
{
	ssize_t hold;

	hold = read(js, &event, sizeof(event));

	if (bytes == sizeof(event))
		return 0;

	/* Error, could not read full event. */
	return -1;
}


//Returns the number of axes on the controller. 
uint8_t Joystick::get_axis_count()
{
    uint8_t axesNum;

    if (ioctl(js, JSIOCGAXES, &axesNum) == -1)
        return 0;

    return axesNum;
}


//Returns the number of buttons on the controller or 0 if an error occurs.
uint8_t Joystick::get_button_count()
{
    uint8_t buttons;
    if (ioctl(js, JSIOCGBUTTONS, &buttons) == -1)
        return 0;

    return buttons;
}



//Returns the axis that the event indicated.
//x = even, y = odd
void Joystick::set_axis_state()
{
    axis = event.number / 2;

    if (axis < 3)
    {
        if (event.number % 2 == 0)
            axes[axis].x = event.value;
        else
            axes[axis].y = event.value;
    }

}


void Joystick::runJoystick() 
{
    while (this->read_event() == 0)
    {
        switch (event.type)
        {
        case JS_EVENT_BUTTON:
            //insert case if button pressed 
            printf("Button %u %s\n", event.number, event.value ? "pressed" : "released");
            break;
        case JS_EVENT_AXIS:
            //insert case if joystick is moved
            this->set_axis_state();
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