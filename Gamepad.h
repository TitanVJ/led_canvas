#ifndef GAMEPAD_H
#define GAMEPAD_H

#include <Xinput.h>

static const WORD XINPUT_Buttons[] = {
      XINPUT_GAMEPAD_A,
      XINPUT_GAMEPAD_B,
      XINPUT_GAMEPAD_X,
      XINPUT_GAMEPAD_Y,
      XINPUT_GAMEPAD_LEFT_THUMB,
      XINPUT_GAMEPAD_RIGHT_THUMB
};

// XInput Button IDs
struct XButtonIDs
{
    XButtonIDs(); //Default constructor

    //buttons
    int A, B, X, Y; 

    // joystick
    int LJoystick;
};


class Gamepad
{
public:	
	//Constructors
	Gamepad();
	Gamepad(int gamePadConnectionIndex);

	void Update();				//Update button state
	void RefreshState();		//Update button states for next frame

	// joystick functions
	// check for joystick activity
	bool LeftDeadzone();

	float LeftX();				//Return X axis of the left joystick
	float LeftY();				//Return Y axis of the left joystick


	bool ButtonPressed(int button);
	bool ButtonDown(int button);

	XINPUT_STATE GetState();	//Return gamepad state 
	int GetGamepadIndex();		//Return gamepad index 
	bool IsConnected();			//return connection status

private:
	X_INPUT_STATE padState;		
	int gamePadIndex;

	static const int buttonCount = 4;		//Total gamepad buttons
	bool prevButtonStates[buttonCount];		//Previous frame button states
	bool currButtonStates[buttonCount];     //Current frame button states
	bool gamepadButtonsDown[buttonCount];	//Buttons pressed on current frame
};

#endif 

// Externally define the 'XButtonIDs' struct as 'XButtons'
extern XButtonIDs XButtons;