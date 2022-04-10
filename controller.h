// Adapted from and heavily inspired by SDL_GameControllerAxis & SDL_GameControlerButton
// Inspired by https://www.youtube.com/watch?v=vbBEG6SvryA&list=WL&index=24&t=488s
// Inspired by https://gist.github.com/jasonwhite/c5b2048c15993d285130



class Joystick
{
public:

	struct axis_state {
		short x, y;
	};

	Joystick();
	Joystick(char* location);
	
	int ReadEvent();
	uint8_t get_axis_count();
	uint8_t get_button_count();
	void set_axis_state();
	void runJoystick();


private:
	struct Controller
	{
		//directory where the device is connected (USB port for now!)
		const char deviceRoute;
		int js;
		Button pressedButton;
		struct js_event event;
		struct axis_state axes[3] = { 0 };
		int axis;
	};

};



//Current state of an axis. (will be moved to controller struct after)
struct axis_state {
	short x, y;
};
