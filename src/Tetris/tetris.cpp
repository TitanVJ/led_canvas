#include "game.hpp"
#include "frame.h"
#include "display.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <vector>
#include "controller.h"

bool running = false;
using LED::Frame;
using LED::Display;

Display display{1, 2, true};

void gameRunner() {
    while(running){
        // get the board
        std::vector<int> tetris_board = CopyBoard();
        std::vector< std::vector<int> > frame_data;
        frame_data.resize( SCREEN_WIDTH );

        auto tetris_itr = tetris_board.begin();
        auto frame_itr = frame_data.begin();

        while( tetris_itr != tetris_board.end() ){
            frame_itr->insert( frame_itr->end(), tetris_itr, tetris_itr + SCREEN_WIDTH );
            ++frame_itr;
            tetris_itr += SCREEN_WIDTH;
        }

        // create a frame
        Frame frame{ frame_data };
        // std::cout << frame << std::endl;

        // update the display
        display.updateFrame( frame );

        // sleep
        std::this_thread::sleep_for( std::chrono::seconds(1) );
    }
}
int main(){
    running = true;
    std::thread game_update_thread{gameRunner};
    Joystick input{};
    game_thread_start();

    // std::string menu =  "Menu(Press the entry + the Enter key)\n"
    //                     "\tR/r: Move piece to RIGHT\n"
    //                     "\tD/d: Move piece DOWN\n"
    //                     "\tL/l: Move piece LEFT\n"
    //                     "\tRO/ro: ROTATE pice\n"
    //                     "\tS/s: Get SCORE\n"
    //                     "\tE/e: Exit the game\n";

        std::string menu =  "Commands\n"
                        "\tB: Move piece to RIGHT\n"
                        "\tA: Move piece DOWN\n"
                        "\tX: Move piece LEFT\n"
                        "\tY: ROTATE pice\n"
                        "\tL-Bumper: Get SCORE\n"
                        "\tR-bumper: Exit the game\n";
    // std::cout << menu << std::endl;
    // menu for inputs
    // std::string usr_input;
    while( true ) {
        std::cout << menu << std::endl;
        // menu tree
        int r = (int) input.check_joystick();

        if( r == 0 ){
            moveTetrominoDown();

        } else if( r == 2 ) {
            moveTetrominoLeft();

        } else if( r == 1  ) {
            moveTetrominoRight();

        } else if( r == 3  ) {
            rotateTetromino();

        } else if( r == 4  ) {
            std::cout << "Current score: " << getScore() << std::endl;

        } else if( r == 5 ) {
            break;
        }

        // if( !usr_input.compare("D") || !usr_input.compare("d") ){
        //     moveTetrominoDown();

        // } else if( !usr_input.compare("L") || !usr_input.compare("l")  ) {
        //     moveTetrominoLeft();

        // } else if( !usr_input.compare("R") || !usr_input.compare("r")  ) {
        //     moveTetrominoRight();

        // } else if( !usr_input.compare("RO") || !usr_input.compare("ro")  ) {
        //     rotateTetromino();

        // } else if( !usr_input.compare("S") || !usr_input.compare("s")  ) {
        //     std::cout << "Current score: " << getScore() << std::endl;

        // } else if( !usr_input.compare("E") || !usr_input.compare("e")  ) {
        //     break;
        // }
    }

    game_thread_stop();
    std::cout << "Final score " << getScore() << std::endl;
    running = false;
    game_update_thread.join();
    return 0;
}