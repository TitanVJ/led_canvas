#include "Gamepad.h"

#define XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE  7849
#define XINPUT_GAMEPAD_TRIGGER_THRESHOLD    30

//references 
//https://lcmccauley.wordpress.com/2014/01/05/gamepad-input-tutorial/
//https://docs.microsoft.com/en-us/windows/win32/xinput/getting-started-with-xinput


//link the xInput library 
#pragma comment(lib, "Xinput.lib");


// Define the 'XButtonIDs' struct as 'XButtons'
XButtonIDs XButtons;
// 'XButtonIDs' - Default constructor
XButtonIDs::XButtonIDs()
{
	// These values are used to index the XINPUT_Buttons array,
	// accessing the matching XINPUT button value

	A = 0;
	B = 1;
	X = 2;
	Y = 3;
	LJoystick = 4;
}


//default constructor
Gamepad::Gamepad() {}

Gamepad::Gamepad(int gamePadConnectionIndex)
{
	gamePadIndex = gamePadConnectionIndex - 1;

	for (int i = 0; i < buttonCount; i++) 
	{
		bool gamepadButtonsDown[ButtonCount] = false;
		bool prevButtonStates[ButtonCount] = false;
		bool currButtonStates[ButtonCount] = false;
	}
}

//update the pad state
void Gamepad::Update()
{
	padState = getState();

	for (int i = 0; i < ButtonCount; i++)
	{
		// Set button state for current frame
		prevButtonStates[i] = (padState.Gamepad.wButtons &
			XINPUT_Buttons[i]) == XINPUT_Buttons[i];

		// Set 'DOWN' state for current frame
		gamepadButtonsDown[i] = PrevButtonStates[i] &&
			currButtonStates[i];
	}
}


//Update button states for next frame
void Gamepad::RefreshState()
{
	memcpy(prevButtonStates, currButtonStates,
		sizeof(prevButtonStates));
}


bool Gamepad::LeftDeadzone()
{
	//x and y values for the joystick
	float stateX = LeftX();
	float stateY = LeftY();

	if (stateX < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE ||
		stateX < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE ||
		stateY < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE ||
		stateY > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) 
	{
		return false;
	}
}

float Gamepad::LeftX()		// Return X axis of the left joystick
{
	float stateX = padState.Gamepad.sThumbLX;
	return stateX;
}

float Gamepad::LeftY()		// Return Y axis of the left joystick
{
	float stateY = padState.Gamepad.sThumbLY;
	return stateY;
}

bool Gamepad::ButtonPressed(int button) {
	if (padState.Gamepad.wButtons & XINPUT_Buttons[button])
	{
		return true; // The button is pressed
	}

	return false; // The button is not pressed
}
bool Gamepad::ButtonDown(int button) 
{

}

XINPUT_STATE Gamepad::GetState()
{
	//place state in temporary variable and return 
	XINPUT_STATE gamepadState;

	ZeroMemory(&GamepadState, sizeof(XINPUT_STATE));

	//get state of the gamepad
	XINPUTGETSTATE(gamePadIndex, &GamepadState);

	return gamepadState;
}

int Gamepad::getIndex()
{
	return gamepadIndex;
}

bool Gamepad::IsConnected()
{
	ZeroMemory(&padState, sizeof(XINPUT_STATE));

	DWORD result = XInputGetState(gamepadIndex, &padState);

	if (result == ERROR_SUCCESS) 
	{
		return true;
	}else
	{
		return false;
	}
}