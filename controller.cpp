// Adapted from and heavily inspired by SDL_GameControllerAxis & SDL_GameControlerButton
// Inspired by https://www.youtube.com/watch?v=vbBEG6SvryA&list=WL&index=24&t=488s
// Inspired by https://gist.github.com/jasonwhite/c5b2048c15993d285130


#include <iostream>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <linux/joystick.h>
#include "controller.h"
#include <time.h>

using input = Joystick::input;

Joystick::Joystick(): axes{ {0, 0}, {0, 0}, {0, 0}}
{
    deviceRoute = "/dev/input/js0";
    js = open(deviceRoute, O_RDONLY);
    if (js == -1)
        perror("Could not open joystick");


    // struct axis_state hold[3] = { 0 };
    // axes = hold;

    inputHold = input::Neutral;
    timeHold = time(0);

}


Joystick::Joystick(char * location)
{
    deviceRoute = location;
    js = open(deviceRoute, O_RDONLY);
    if (js == -1)
        perror("Could not open joystick");

    // struct axis_state hold[3] = { 0 };
    // axes = hold;
    axes[0] = { 0, 0};
    axes[1] = { 0, 0};
    axes[2] = { 0, 0};

    inputHold = input::Neutral;
}

Joystick::~Joystick()
{
    close(js);
}


int Joystick::read_event()
{
	ssize_t hold, bytes;

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
int Joystick::get_axis_state()
{
    int axis = event.number / 2;

    if (axis < 3)
    {
        if (event.number % 2 == 0)
            axes[axis].x = event.value;
        else
            axes[axis].y = event.value;
    }
    return axis;
}


input Joystick::check_joystick()
{
    int axis;

    this->read_event();
    switch (event.type)
    {
        case JS_EVENT_BUTTON:
            //insert case if button pressed
            printf("Button %u %s\n", event.number, event.value ? "pressed" : "released");

            switch (event.number)
            {
                case 0:
                    inputHold = input::A;
                case 1:
                    inputHold = input::B;
                case 2:
                    inputHold = input::X;
                case 3:
                    inputHold = input::Y;
            }

        case JS_EVENT_AXIS:
            if (true)
            {


                timeHold = time(0);
                //insert case if joystick is moved
                axis = this->get_axis_state();
                if (axis < 3)
                    printf("Axis %zu at (%6d, %6d)\n", axis, axes[axis].x, axes[axis].y);

                //Please add cases for directional inputs
            }
            else
            {
                inputHold = input::Neutral;
            }

            default:
                /* Ignore init events. */
                inputHold = input::Neutral;
    }

        fflush(stdout);
        return inputHold;
}
