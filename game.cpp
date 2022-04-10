/*https://www.youtube.com/watch?v=8OK8_tHeCIA&t=1030s*/

#include <iostream>
#include <thread>

using namespace std;

static string tetromino [7];
static int screenWidth = 12;
static int screenHeight = 18;
static char * screenBuffer = nullptr; // actual final data
static char * displayScreen = nullptr;
static bool gameOver = false;

static int currentPiece = 0; //rand() % 7
static int currentRotation = 0;
static int currentX = screenWidth/2 - 2;
static int currentY = 1;


static bool movingDown = false;

static int difficultySpeed = 20;
static int difficultySpeedCounter = 0;
static int countTetromino = 0;
static int score = 0;



static void init_tetrisTetromino(void){
    /*
    tetromino[0].append("..X.");
    tetromino[0].append("..X.");
    tetromino[0].append("..X.");
    tetromino[0].append("..X.");

    tetromino[1].append("..X.");
    tetromino[1].append(".XX.");
    tetromino[1].append(".X..");
    tetromino[1].append("....");

    tetromino[2].append(".X..");
    tetromino[2].append(".XX.");
    tetromino[2].append("..X.");
    tetromino[2].append("....");

    tetromino[3].append("....");
    tetromino[3].append(".XX.");
    tetromino[3].append(".XX.");
    tetromino[3].append("....");

    tetromino[4].append("..X.");
    tetromino[4].append(".XX.");
    tetromino[4].append("..X.");
    tetromino[4].append("....");

    tetromino[5].append("....");
    tetromino[5].append(".XX.");
    tetromino[5].append("..X.");
    tetromino[5].append("..X.");

    tetromino[6].append("...");
    tetromino[6].append(".XX.");
    tetromino[6].append(".X..");
    tetromino[6].append(".X..");
    */

    tetromino[0].append("..X...X...X...X.");
    tetromino[1].append("..X..XX..X......");
    tetromino[2].append(".X...XX...X.....");
    tetromino[3].append(".....XX..XX.....");
    tetromino[4].append("..X..XX...X.....");
    tetromino[5].append(".....XX...X...X.");
    tetromino[6].append("....XX..X...X..");
}

static void init_tetrisBoard(void){
     // setting the board 
    displayScreen = new char[screenHeight*screenWidth];
    screenBuffer = new char[screenHeight*screenWidth];
    for(int x = 0; x < screenWidth; x++){
        for (int y = 0; y < screenHeight; y++){
            if( x == 0 || x == screenWidth -1 || y == 0 || y == screenHeight - 1){
                displayScreen[y*screenWidth + x] = '#';
                screenBuffer[y*screenWidth + x] = '#';
            }
            else{
                displayScreen[y*screenWidth + x] = ' ';
                screenBuffer[y*screenWidth + x] = ' ';
            }
            //displayScreen[y*screenWidth + x] = ' ';
        }
        
    }
    for(int x = 1; x < screenWidth - 1; x++){
        screenBuffer[(screenHeight-3)*screenWidth + x] = 'B';
        screenBuffer[(screenHeight-4)*screenWidth + x] = 'C';
        screenBuffer[(screenHeight-2)*screenWidth + x] = 'D';
    }
    screenBuffer[(screenHeight-3)*screenWidth + 6] = ' ';
    screenBuffer[(screenHeight-4)*screenWidth + 6] = ' ';
    screenBuffer[(screenHeight-2)*screenWidth + 6] = ' ';
    screenBuffer[(screenHeight-2)*screenWidth + 3] = ' ';
    
}

static int getScore(void){
    return score;
}

static int blockPosition(int px, int py, int rotate){
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

static bool doesTetrominoFit(int numTetromino, int numRotation, int posX, int posY){
    for(int x = 0; x < 4; x++){
        for(int y = 0; y < 4; y++){
            // Get index of tetromino's block
            int blockIndex = blockPosition(x, y, numRotation);
            // Get index of screen
            int screenIndex = (posY + y) * screenWidth + (posX + x);

            // check for out of bound
            if(posX + x >= 0 && posX + x < screenWidth){
                if(posY + y >= 0 && posY + y < screenHeight){
                    // check collision
                    //cout << "hey" << endl;
                    if(tetromino[numTetromino][blockIndex] == 'X' && screenBuffer[screenIndex] != ' '){
                        //cout << "hey bad news" << endl;
                        //cout << screenBuffer[screenIndex] << endl;
                        //cout << tetromino[numTetromino][blockIndex] << endl;
                        return false;
                    }
                }
            }
        }
    }
    return true;
}

static void moveTetrominoLeft(void){
    //cout << "yes"<< endl; 
    if (doesTetrominoFit(currentPiece, currentRotation, currentX - 1, currentY)){
        //cout << posX << endl;
        currentX -= 1;
        //cout << currentX << endl; 
    }
}
static void moveTetrominoRight(void){
    if (doesTetrominoFit(currentPiece, currentRotation, currentX + 1, currentY)){
        currentX += 1; 
    }
}
static void moveTetrominoDown(void){
    if (doesTetrominoFit(currentPiece, currentRotation, currentX, currentY + 1)){
        currentY += 1; 
    }
}

static void rotateTetromino(void){
    //cout << currentPiece << " " << currentRotation + 1 << " " << currentX << " " << currentY << endl;
    if (doesTetrominoFit(currentPiece, currentRotation + 1, currentX, currentY)){
        currentRotation = currentRotation + 1;
        //cout << currentRotation << endl;
    }
}

static void copyScreen(void){
    for(int x = 0; x < screenWidth; x++){
        for (int y = 0; y < screenHeight; y++){
                displayScreen[y*screenWidth + x] = screenBuffer[y*screenWidth + x];
        }   
    }
}

/* Temporary fuctions for debugging */
static void printScreen(void){
    cout << score << endl;
    for(int x = 0; x < screenHeight; x++){
        for (int y = 0; y < screenWidth; y++){
            cout << displayScreen[x*screenWidth + y];
            cout << " ";
        }
        cout << endl;
    }
}


int main(){

    //cout << "Hello World" << endl;

    init_tetrisTetromino();


    init_tetrisBoard();

    

    while(!gameOver){

        // Timing =======================
		this_thread::sleep_for(50ms); // Small Step = 1 Game Tick
        difficultySpeedCounter++;
        if(difficultySpeed == difficultySpeedCounter){
            movingDown = true;
            //cout << "here" << endl;
        }
        else{
            movingDown = false;
        }

        
        // Input ========================
		
        // Game Logic ========================
        //copyScreen();
        //moveTetrominoLeft();
        //moveTetrominoLeft();
        //moveTetrominoLeft();
       //rotateTetromino();
        copyScreen();
        //rotateTetromino();
        //copyScreen();
        //rotateTetromino();
        
        if(movingDown){
            difficultySpeedCounter = 0;
            if(doesTetrominoFit(currentPiece, currentRotation, currentX, currentY + 1)){
                currentY += 1;
            }
            else{
                countTetromino++;
                if(countTetromino % 10 == 0){
                    if(difficultySpeed >= 10){
                        difficultySpeed--; // the smaller difficultySpeed the faster the game moves down the harder game gets
                    }
                }
                // lock the current block to buffer
                for(int x = 0; x < 4; x++){
                    for (int y = 0; y < 4; y++){
                        if(tetromino[currentPiece][blockPosition(x, y, currentRotation)] == 'X'){
                            screenBuffer[(currentY + y) * screenWidth + (currentX + x)] = currentPiece + 65;
                        }
                    }
                }
                // check for horizontal lines
                for(int y = 0; y < 4; y++){
                    if(currentY + y < screenHeight - 1){
                        bool tetrominoLine = true;
                        for(int x = 1; x < screenWidth - 1; x++){
                            if(screenBuffer[(currentY + y) * screenWidth + x] == ' '){
                                tetrominoLine = false;
                            }
                        }
                        if(tetrominoLine){
                            score += 10;
                            //////////////////////////////////////////////////////////////// DONT ERASE
                            /*for(int x = 1; x < screenWidth - 1; x++){
                               screenBuffer[(currentY + y) * screenWidth + x] = '=';
                            }*/
                            for (int px = 1; px < screenWidth - 1; px++)
                            {
                                for (int py = currentY + y; py > 1; py--){
                                    screenBuffer[py * screenWidth + px] = screenBuffer[(py - 1) * screenWidth + px];
                                }
                                    //screenBuffer[py * screenWidth + px] = screenBuffer[(py - 1) * screenWidth + px];
                                screenBuffer[px] = ' ';
                            }
                        
                            /*for(int x = 1; x < screenWidth - 1; x++){
                                //screenBuffer[(currentY + y) * screenWidth + x] = screenBuffer[(currentY + y - 1) * screenWidth + x];
                                //cout << currentY + y <<  "                                       \n\n\n\n\n" <<endl;
                                
                            }*/
                           
                        }
                    }
                }
                copyScreen();
                //copyScreen();
                //printScreen();
                //return 0;
                // choose next block
                score += 5;
                currentPiece = rand() % 7;
                currentRotation = 0;
                currentX = screenWidth/2 - 1;
                currentY = 1;

                // if block cant fit --> gameOver
                if(!doesTetrominoFit(currentPiece, currentRotation, currentX, currentY)){
                    gameOver = true;
                }
            }
        }
        
        // put the tetris in screen
        for(int x = 0; x < 4; x++){
            for (int y = 0; y < 4; y++){
                if(tetromino[currentPiece][blockPosition(x, y, currentRotation)] == 'X'){
                    //cout << tetromino[currentPiece][blockPosition(x, y, currentRotation)] << endl;
                    //cout << (currentY + y) * screenWidth + (currentX + x) << endl;
                    displayScreen[(currentY + y) * screenWidth + (currentX + x)] = currentPiece + 65;
                }
            }
        }

        // print for testing
        printScreen();
        //break;
    }

    cout << "Final Score: " << score << " GG!" << endl;
 
    return 0;
}