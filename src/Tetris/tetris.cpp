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
    // Mic_startListening();
    game_thread_start();


        std::string menu =  "Commands\n"
                        "\tB: Move piece to RIGHT\n"
                        "\tA: Move piece DOWN\n"
                        "\tX: Move piece LEFT\n"
                        "\tY: ROTATE pice\n"
                        "\tL-Bumper: Get SCORE\n"
                        "\tR-bumper: Exit the game\n";

    std::cout << menu << std::endl;
    while( true ) {
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
    }

    game_thread_stop();
    std::cout << "Final score " << getScore() << std::endl;
    running = false;
    game_update_thread.join();
    return 0;
}