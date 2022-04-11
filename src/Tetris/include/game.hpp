/*
*  Creates a thread that runs the game and handles everything realted to the logic of the game
*  gets a copy of the game board
*  gives the current score
*  allows access to the tetromino's movements
*/

#include <vector>

#define SCREEN_WIDTH 32
#define SCREEN_HEIGHT 32

#ifndef _GAME_H_
#define _GAME_H_

//Start Thread
void game_thread_start(void);

//Join Thread (close thread)
void game_thread_stop(void);

//Copy Screen Board
std::vector <int> CopyBoard(void);

// Getting the Score
int getScore(void);

// Moving Tetrominos
void moveTetrominoLeft(void);
void moveTetrominoRight(void);
void moveTetrominoDown(void);
void rotateTetromino(void);
bool isGameOver(void);

#endif