#include "game.hpp"
#include <iostream>
#include <chrono>
#include <thread>
#include <vector>

bool running = false;

void gameRunner() {
    while(running){
        // get the board
        std::vector<int> tetris_board = CopyBoard();
        std::vector< std::vector<int> > frame_data;
        frame_data.reserve( SCREEN_HEIGHT );

        auto tetris_itr = tetris_board.begin();
        auto frame_itr = frame_data.begin();
        while( tetris_itr != tetris_board.end() ){
            frame_itr->insert( frame_itr->end(), tetris_itr, tetris_itr + SCREEN_WIDTH );
            ++frame_itr;
            tetris_itr += SCREEN_WIDTH;
        }

        // create a frame

        // update the display

        // sleep

        std::this_thread::sleep_for( std::chrono::seconds(1) );
    }
}
int main(){
    running = true;
    std::thread game_update_thread{gameRunner};
    game_thread_start();

    // std::string s;
    // while(true){
    // std::this_thread::sleep_for(std::chrono::seconds(1));
    // // moveTetrominoDown();

    // std::vector<int> board = CopyBoard();
    //     int i = 1;
    //     for(auto value : board) {
    //         std::cout << value;
    //         if(i %32 == 0){
    //             std::cout << std::endl;
    //         }
    //         ++i;
    //     }

    // }

    // menu for inputs
    std::string usr_input;
    while( !isGameOver() && std::cin >> usr_input ) {

        // menu tree
        if( !usr_input.compare("D") || !usr_input.compare("d") ){
            moveTetrominoDown();

        } else if( !usr_input.compare("L") || !usr_input.compare("l")  ) {
            moveTetrominoLeft();

        } else if( !usr_input.compare("R") || !usr_input.compare("r")  ) {
            moveTetrominoRight();

        } else if( !usr_input.compare("RO") || !usr_input.compare("ro")  ) {
            rotateTetromino();

        } else if( !usr_input.compare("S") || !usr_input.compare("s")  ) {
            std::cout << "Current score: " << getScore() << std::endl;

        } else if( !usr_input.compare("E") || !usr_input.compare("e")  ) {
            break;
        }
    }

    game_thread_stop();
    std::cout << "Final score " << getScore() << std::endl;
    running = false;
    game_update_thread.join();
    return 0;
}