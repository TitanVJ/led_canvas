#include <iostream>
#include "controller.h"
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <linux/joystick.h>

//NOTE: does not use the controller class -
//I do not know the buttons and their names yet
//When the inputs are tested, you can use that to get the proper enum classs
using input = Joystick::input;

int main(int argc, char* argv[]) {
    Joystick test;
    // input result;
    // result = test.check_joystick();
    // std::cout << (int)result << "\n";
    // result = test.check_joystick();
    // std::cout << (int)result << "\n";
    // result = test.check_joystick();
    // std::cout << (int)result << "\n";

    while(true){
        test.check_joystick();
        // std::cout << (int)result << std::endl;
    }
    return(0);
}