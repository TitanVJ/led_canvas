// Adapted from and heavily inspired by SDL_GameControllerAxis & SDL_GameControlerButton
// Inspired by https://www.youtube.com/watch?v=vbBEG6SvryA&list=WL&index=24&t=488s
// Inspired by https://gist.github.com/jasonwhite/c5b2048c15993d285130

#pragma once
#include <linux/joystick.h>

class Joystick
{
public:

	struct axis_state {
		short x, y;
	};


	// Adapted from SDL_GameControllerButton
	enum class input
	{
		Up,
		Down,
		Left,
		Right,
		A,
		B,
		X,
		Y,
		Neutral
	};

	Joystick();
	Joystick(char* location);
	~Joystick();

	int read_event();
	uint8_t get_axis_count();
	uint8_t get_button_count();
	int get_axis_state();
	input check_joystick();



private:
	const char* deviceRoute;
	int js;
	struct js_event event;
	struct axis_state axes[3];
	input inputHold;
	time_t timeHold;


};



//Current state of an axis. (will be moved to controller struct after)
struct axis_state {
	short x, y;
};
