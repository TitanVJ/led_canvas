// Adapted from and heavily inspired by SDL_GameControllerAxis & SDL_GameControlerButton
// Inspired by https://www.youtube.com/watch?v=vbBEG6SvryA&list=WL&index=24&t=488s
// Inspired by https://gist.github.com/jasonwhite/c5b2048c15993d285130


namespace joystick::input
{
	class Joystick
	{
	public:
		// Adapted from SDL_GameControllerAxis
		enum class Axis
		{
			LeftStickX,
			LeftStickY
		};

		// Adapted from SDL_GameControllerButton
		enum class Button
		{
			A,
			B,
			X,
			Y
		};

	public:
		//may comment out for now incase it gives errors
		/*
		static void ReadControllerEvent(struct js_event* event);
		static void CheckButton(struct js_event* event);
		static void CheckAxisState(struct js_event* event);

		*/

	private:
		struct Controller
		{
			//directory where the device is connected (USB port for now!)
			const char deviceRoute;
			Axis stick;
			Button pressedButton;
		};

	};
}


//Current state of an axis. (will be moved to controller struct after)
struct axis_state {
	short x, y;
};

// SDL_GameControllerAxis
// SDL_GameControlerButton

//reads event from joystick
int ReadEvent(int fd, struct js_event* event);

//Returns the number of axes on the controller. 
int get_axis_count(int fd);

//Returns the number of buttons on the controller or 0 if an error occurs.
int get_button_count(int fd);

int get_axis_state(struct js_event* event, struct axis_state axes[3]);