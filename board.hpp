#ifndef BOARD_H
#define BOARD_H

#include <cstdint>

const int ROWS = 6;
const int COLS = 7;
const int TARGET = 4;

class BitBoard
{
public:
    // PUBLIC VARIABLES
    // TranspositionTable tt;
    char currentPlayer;
    char oppPlayer;
    
    static_assert
    (
        (ROWS+1)*COLS <= 64, 
        "UNSUCCESSFUL Board declaration!\nDimensions don't fit in 64-bits"
    );


    // PUBLIC FUNCTIONS

    BitBoard();
    ~BitBoard();

    // get Kth bit of bitboard key
    int getBit(uint64_t num, int k);
    uint64_t getKey();
    
    void printBitBoard();
    void printBoard();
    void printMoveHistory();

    void switchPlayers();
    int getMove();
    bool isPlayable(int col);

    void playMove(int col);
    void unPlayMove(int col);

    int evalBoard();
    bool isWin();
    bool isTied();


private:
    // PRIVATE VARIABLES
    
    // horizontal, vertical, majorDiagonal, minorDiagonal
    int directions[4] = {ROWS + 1, 1, ROWS, ROWS + 2};

    // board representation for current player occupied cells
    uint64_t currentPosition;
    
    // board representation for all occupied cells
    uint64_t mask;
    
    // bottom Mask
    uint64_t bottomMask;
    
    // idx for value 1 depicting empty cell;
    int emptyOne[COLS];
    
    int moveHistory[ROWS*COLS];
    int move;

    // PRIVATE FUNCTIONS

    void initVariables();
    int countPieces(uint64_t pos);
    int getScore(int linesCount[2][TARGET/2]);



};

#endif
