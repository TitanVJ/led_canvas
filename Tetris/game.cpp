/* This game was done with the help of this video https://www.youtube.com/watch?v=8OK8_tHeCIA&t=1030s
The logic of the game is taken from the video*/

#include <iostream>
#include <thread>
#include <vector>
#include <pthread.h>
#include "game.hpp"

using namespace std;

// Display Screen Colors
#define BLANK_BOARD_COLOR 0
#define BORDER_COLOR 7
static vector <int> TetrominoColor = {1, 2, 3, 4, 5, 6};

// Tetromino Pieces
static string tetromino [7];

// Boards
static vector <int> screenBuffer(SCREEN_HEIGHT*SCREEN_WIDTH, BLANK_BOARD_COLOR); // actual final Board
static vector <int> displayScreen(SCREEN_HEIGHT*SCREEN_WIDTH, BLANK_BOARD_COLOR);

static bool gameOver = false;

static int currentPiece = rand() % 7;
static int currentRotation = 0;
static int currentX = SCREEN_WIDTH/2 - 2;
static int currentY = 1;


static bool movingDown = false;

static int difficultySpeed = 20;
static int difficultySpeedCounter = 0;
static int countTetromino = 0;
static int score = 0;

static pthread_t id1;

static void init_tetrisTetromino(void)
{
    tetromino[0].append("..X...X...X...X.");
    tetromino[1].append("..X..XX..X......");
    tetromino[2].append(".X...XX...X.....");
    tetromino[3].append(".....XX..XX.....");
    tetromino[4].append("..X..XX...X.....");
    tetromino[5].append(".....XX...X...X.");
    tetromino[6].append("....XX..X...X..");
}

static void init_tetrisBoard(void)
{
    //Setting the board
    for(int x = 0; x < SCREEN_WIDTH; x++){
        for (int y = 0; y < SCREEN_HEIGHT; y++){
            if( x == 0 || x == SCREEN_WIDTH -1 || y == 0 || y == SCREEN_HEIGHT - 1){
                displayScreen[y*SCREEN_WIDTH + x] = BORDER_COLOR;
                screenBuffer[y*SCREEN_WIDTH + x] = BORDER_COLOR;
            }
        }

    }
}

static int blockPosition(int px, int py, int rotate)
{
    switch (rotate % 4)
    {
    case 0: // 0 Degree
       return py * 4 + px;

    case 1: // 90 Degree
       return 12 + py - (px * 4);

    case 2: // 180 Degree
       return 15 - (py * 4) - px;

    case 3: // 270 Degree
       return 3 - py + (px * 4);

    default:
        return 0;
    }
}

static bool doesTetrominoFit(int numTetromino, int numRotation, int posX, int posY)
{
    for(int x = 0; x < 4; x++){
        for(int y = 0; y < 4; y++){
            // Get index of tetromino's block
            int blockIndex = blockPosition(x, y, numRotation);
            // Get index of screen
            int screenIndex = (posY + y) * SCREEN_WIDTH + (posX + x);

            // check for out of bound
            if(posX + x >= 0 && posX + x < SCREEN_WIDTH){
                if(posY + y >= 0 && posY + y < SCREEN_HEIGHT){
                    // check collision
                    if(tetromino[numTetromino][blockIndex] == 'X' && screenBuffer[screenIndex] != BLANK_BOARD_COLOR){
                        return false;
                    }
                }
            }
        }
    }
    return true;
}
// Copy actual board to display board
static void copyScreen(void)
{
    for(int x = 0; x < SCREEN_WIDTH; x++){
        for (int y = 0; y < SCREEN_HEIGHT; y++){
                displayScreen[y*SCREEN_WIDTH + x] = screenBuffer[y*SCREEN_WIDTH + x];
        }
    }
}

int getScore(void)
{
    return score;
}

void moveTetrominoLeft(void)
{
    if (doesTetrominoFit(currentPiece, currentRotation, currentX - 1, currentY))
    {
        currentX -= 1;
    }
}

void moveTetrominoRight(void)
{
    if (doesTetrominoFit(currentPiece, currentRotation, currentX + 1, currentY)){
        currentX += 1;
    }
}
void moveTetrominoDown(void)
{
    if (doesTetrominoFit(currentPiece, currentRotation, currentX, currentY + 1)){
        currentY += 1;
    }
}

void rotateTetromino(void)
{
    if (doesTetrominoFit(currentPiece, currentRotation + 1, currentX, currentY)){
        currentRotation = currentRotation + 1;
    }
}

// a copy of the game board
vector <int> CopyBoard(void){
    vector <int> copyGameBoard(SCREEN_HEIGHT*SCREEN_WIDTH, BLANK_BOARD_COLOR);
    for(int i = 0; i < SCREEN_HEIGHT*SCREEN_WIDTH; i++){
       copyGameBoard[i] = displayScreen[i];
    }
    return copyGameBoard;
}

/* Temporary fuctions for debugging */
/*
static void printScreen(void)
{
    cout << score << endl;
    for(int x = 0; x < SCREEN_HEIGHT; x++){
        for (int y = 0; y < SCREEN_WIDTH; y++){
            cout << displayScreen[x*SCREEN_WIDTH + y];
            cout << " ";
        }
        cout << endl;
    }
}
*/

static void * StartGame(void * arg)
{

    init_tetrisTetromino();
    init_tetrisBoard();

    while(!gameOver){

        /*                      *
        *    Iteration Timing   *
        *                       */
		this_thread::sleep_for(50ms);
        difficultySpeedCounter++;
        if(difficultySpeed == difficultySpeedCounter){
            movingDown = true;
        }
        else{
            movingDown = false;
        }

        /*                *
        *    Game Logic   *
        *                */
        copyScreen();

        if(movingDown){
            difficultySpeedCounter = 0;
            if(doesTetrominoFit(currentPiece, currentRotation, currentX, currentY + 1)){
                currentY += 1;
            }
            else{
                countTetromino++;
                if(countTetromino % 10 == 0){
                    if(difficultySpeed >= 10){
                        difficultySpeed--; // the smaller difficultySpeed, the faster the game moves down, the harder game gets
                    }
                }
                // lock the current block to buffer
                for(int x = 0; x < 4; x++){
                    for (int y = 0; y < 4; y++){
                        if(tetromino[currentPiece][blockPosition(x, y, currentRotation)] == 'X'){
                            screenBuffer[(currentY + y) * SCREEN_WIDTH + (currentX + x)] = TetrominoColor[currentPiece%TetrominoColor.size()];
                        }
                    }
                }
                // check for horizontal lines
                for(int y = 0; y < 4; y++){
                    if(currentY + y < SCREEN_HEIGHT - 1){
                        bool tetrominoLine = true;
                        for(int x = 1; x < SCREEN_WIDTH - 1; x++){
                            if(screenBuffer[(currentY + y) * SCREEN_WIDTH + x] == BLANK_BOARD_COLOR){
                                tetrominoLine = false;
                            }
                        }
                        if(tetrominoLine){
                            score += 10;
                            /*for(int x = 1; x < SCREEN_WIDTH - 1; x++){
                               screenBuffer[(currentY + y) * SCREEN_WIDTH + x] = BORDER_COLOR;
                            }*/
                            for (int px = 1; px < SCREEN_WIDTH - 1; px++)
                            {
                                for (int py = currentY + y; py > 1; py--){
                                    screenBuffer[py * SCREEN_WIDTH + px] = screenBuffer[(py - 1) * SCREEN_WIDTH + px];
                                }

                            }
                        }
                    }
                }
                copyScreen();
                // choose next block
                score += 5;
                currentPiece = rand() % 7;
                currentRotation = 0;
                currentX = SCREEN_WIDTH/2 - 1;
                currentY = 1;

                // if block can't fit --> gameOver
                if(!doesTetrominoFit(currentPiece, currentRotation, currentX, currentY)){
                    gameOver = true;
                }
            }
        }


        // put the tetris in screen
        for(int x = 0; x < 4; x++){
            for (int y = 0; y < 4; y++){
                if(tetromino[currentPiece][blockPosition(x, y, currentRotation)] == 'X'){
                    displayScreen[(currentY + y) * SCREEN_WIDTH + (currentX + x)] = TetrominoColor[currentPiece%TetrominoColor.size()];
                }
            }
        }

        //Print Board for Testing
        //printScreen();
    }

    //cout << "Final Score: " << score << " GG!" << endl;

    return nullptr;
}

void game_thread_start(void)
{
    pthread_create(&id1, NULL, &StartGame, NULL);
}

void game_thread_stop(void)
{
    pthread_join(id1, NULL);
}

bool isGameOver(void) { return gameOver; }