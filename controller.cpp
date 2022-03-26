// Adapted from and heavily inspired by SDL_GameControllerAxis & SDL_GameControlerButton
// Inspired by https://www.youtube.com/watch?v=vbBEG6SvryA&list=WL&index=24&t=488s
// Inspired by https://gist.github.com/jasonwhite/c5b2048c15993d285130


#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <linux/joystick.h>
#include "controller.h"

int ReadEvent(int fd, struct js_event* event)
{
	ssize_t hold;

	hold = read(fd, event, sizeof(*event));

	if (bytes == sizeof(*event))
		return 0;

	/* Error, could not read full event. */
	return -1;
}


//Returns the number of axes on the controller. 
int get_axis_count(int fd)
{
    uint8_t axes;

    if (ioctl(fd, JSIOCGAXES, &axes) == -1)
        return 0;

    return axes;
}


//Returns the number of buttons on the controller or 0 if an error occurs.
int get_button_count(int fd)
{
    uint8_t buttons;
    if (ioctl(fd, JSIOCGBUTTONS, &buttons) == -1)
        return 0;

    return buttons;
}





//Returns the axis that the event indicated.
//x = even, y = odd
int get_axis_state(struct js_event* event, struct axis_state axes[3])
{
    int axis = event->number / 2;

    if (axis < 3)
    {
        if (event->number % 2 == 0)
            axes[axis].x = event->value;
        else
            axes[axis].y = event->value;
    }

    return axis;
}