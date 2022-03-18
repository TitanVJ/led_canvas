#include <iostream>
#include "Gamepad.h"

using namespace std;

int main() {
    Gamepad test = Gamepad(1);

    // A function to obtain input, called each frame
    void GetGamepadInput()
    {
        test.Update(); // Update the gamepad

        if (test.GetButtonPressed(XButtons.A)) 
        {
            cout << "a pressed";
        }

        if (test.GetButtonDown(XButtons.X))
        {
            cout << "x pressed";
        }
        
        if (test.GetButtonDown(XButtons.Y))
        {
            cout << "y pressed";
        }

        if (test.GetButtonDown(XButtons.B))
        {
            cout << "B pressed";
        }

        test.RefreshState();
}