#include <iostream>
#include <math.h>
#include "board.hpp"


//PUBLIC FUNCTIONS
BitBoard :: BitBoard()
{
    this->initVariables();
}


BitBoard :: ~BitBoard()
{
    //
}


int BitBoard :: getBit(uint64_t num, int k)
{
    uint64_t mask = UINT64_C(1) << k;
    return ((num & mask) >> k);
}


uint64_t BitBoard :: getKey()
{
    uint64_t key = currentPosition + mask + bottomMask;
    return key;
}


void BitBoard :: printBitBoard()
{
    uint64_t key = currentPosition ^ mask;
    int shiftFactor;
    
    for (int j = 0; j <= ROWS; j++)
    {
        for (int i = 0; i < COLS; i++)
        {
            shiftFactor = (i * COLS) + (ROWS - j);
            std::cout << getBit(key, shiftFactor) << " ";
        }
        std::cout << std::endl;
    }

    for (int i = 1; i <= COLS; i++)
    {
        std::cout << "-" << " ";
    }
    std::cout << "\n" << std::endl;

    return;
}


void BitBoard :: printBoard()
{   
    uint64_t key = getKey();
    int shiftFactor;
    
    for (int j = 1; j <= ROWS; j++)
    {
        std::cout << j << "| ";
        for (int i = 0; i < COLS; i++)
        {
            shiftFactor = (i * COLS) + (ROWS - j);
            if (j <= emptyOne[i])
            {
                std::cout << '_' << " | ";
            }
            else if (getBit(key, shiftFactor) == 1)
            {
                //since player switch before rendering
                std::cout << oppPlayer << " | ";
            }
            else
            {
                std::cout << currentPlayer << " | ";
            }
        }
        std::cout << std::endl;
    }

    for (int i = 1; i <= COLS; i++)
    {
        std::cout << "   " << i;
    }
    std::cout << "\n" << std::endl;
    
    return;
}


void BitBoard :: printMoveHistory()
{
    if (this->move > 1)
    {
        std::cout << "Moves Played :: ";
        //since rendering after a move, hence move increases
        //so move - 2 is last index to be printed
        for (int i = 0; i < move - 1; i++)
        {
            std::cout << moveHistory[i] << " ";
        }
        std::cout << std::endl;
    }
    return;
}


void BitBoard :: switchPlayers()
{
    if (currentPlayer == 'O')
    {
        currentPlayer = 'X';
        oppPlayer = 'O';
    }
    else
    {
        currentPlayer = 'O';
        oppPlayer = 'X';
    }
    return;
}


int BitBoard :: getMove()
{
    return move;
}


bool BitBoard :: isPlayable(int col)
{
    return (emptyOne[col] > 0);
    // uint64_t key = getKey();
    // int shiftFactor = ((col + 1) * (ROWS + 1)) - 1;
    // return (getBit(key, shiftFactor) == 0);
}


void BitBoard :: playMove(int col)
{
    int shiftFactor = (col * (ROWS + 1)) + (ROWS - emptyOne[col]);
    mask |= (UINT64_C(1) << shiftFactor);
    currentPosition ^= mask;
    emptyOne[col]--;
    moveHistory[move - 1] = col;
    move++;

    // int player = (currentPlayer == 'O' ? 0 : 1);
    // tt.zobHash.zobKey ^= tt.zobHash.ZobristTable[col][ROWS - emptyOne[col]][player];

    return;
}


void BitBoard :: unPlayMove(int col)
{
    int shiftFactor = (col * (ROWS + 1)) +  (ROWS - emptyOne[col] - 1);
    mask ^= (UINT64_C(1) << shiftFactor);
    currentPosition ^= (UINT64_C(1) << shiftFactor);
    currentPosition ^= mask;
    emptyOne[col]++;
    moveHistory[move - 1] = -1;
    move--;

    // int player = (currentPlayer == 'O' ? 0 : 1);
    // tt.zobHash.zobKey ^= tt.zobHash.ZobristTable[col][ROWS - emptyOne[col] - 1][player];

    switchPlayers();
    return;
}


int BitBoard :: evalBoard(bool maximizer)
{
    int score = 0;

    int linesCount[2][TARGET / 2];
    int idx;
    int pieceCount, oppPieceCount;
    //for each direction
    for (int i = 0; i < 4; i++)
    {
        uint64_t pos = currentPosition;
        uint64_t oppPos = currentPosition ^ mask;
        idx = 0;
        
        //shift to ensure atleast TARGET / 2 in a direction
        for (int j = 1; j < TARGET / 2; j++)
        {
            pos = pos & (pos >> directions[i]);
            oppPos = oppPos & (oppPos >> directions[i]);
        }

        pieceCount = countPieces(pos);
        linesCount[0][idx] = pieceCount;
        oppPieceCount = countPieces(oppPos);
        linesCount[1][idx] = oppPieceCount;
        
        //store number of x-in a direction
        while (idx < TARGET / 2)
        {
            pos = pos & (pos >> directions[i]);
            pieceCount = countPieces(pos);
            linesCount[0][idx] -= pieceCount;

            oppPos = oppPos & (oppPos >> directions[i]);
            oppPieceCount = countPieces(oppPos);
            linesCount[1][idx] -= oppPieceCount;

            linesCount[0][++idx] = pieceCount;
            linesCount[1][idx] = oppPieceCount;
        }
    }
    //now lineCount stores all posssible x in a row
    //generate static linear evaluation formula
    score = getScore(linesCount);

    //find who is maximizing player and change score;
    int player = (currentPlayer == 'O' ? -1 : 1);
    int maximizerInt = (maximizer == true ? 1 : -1);
    
    score *= (player * maximizerInt);


    // player = maximizing player
    // if (maximizer == true && player == -1)
    // {
    //     player = 1;
    // }
    // else if (maximizer == true && player == 1)
    // {
    //     player = -1;
    // }
    // else if (maximizer == false && player == 1)
    // {
    //     player = 1;
    // }
    // else if (maximizer == false && player == -1)
    // {
    //     player = -1;
    // }

    
    // if (player != 1)
    // {
    //     score *= -1;
    // }
    
    return score;
}


bool BitBoard :: isWin()
{
    for (int i = 0; i < 4; i++)
    {
        uint64_t pos = currentPosition;
        for (int j = 1; j < TARGET; j++)
        {
            pos = pos & (pos >> directions[i]);
        }
        if (pos != 0)
        {
            return true;
        }
    }
    
    return false;
}

bool BitBoard :: isTied()
{
    return ((move - 1) == (ROWS * COLS));
}


//PRIVATE FUNCTIONS

void BitBoard :: initVariables()
{
    this->currentPosition = 0;
    this->mask = 0;
    this->bottomMask = 0;
    for (int j = 0; j < COLS; j++)
    {
        bottomMask += (UINT64_C(1) << (j * (ROWS+1)));
        emptyOne[j] = ROWS;
        
        for (int i = 0; i < ROWS; i++)
        {
            moveHistory[j*COLS + i] = -1;
        }
    }
    this->move = 1;
    this->currentPlayer = 'O';
    this->oppPlayer = 'X';
    return;
}


int BitBoard :: countPieces(uint64_t pos)
{
    int count = 0;

    while (pos)
    {
        if (pos & 1)
        {
            count++;
        }
        pos >>= 1;
    }

    return count;
}


int BitBoard :: getScore(int linesCount[2][TARGET/2])
{
    int score = 0;
    int factorVal = 0;

    for (int i = 0; i < 2; i++)
    {
        factorVal = TARGET + 1;
        factorVal <<= 1;

        for (int j = 0; j < TARGET/2; j++)
        {
            factorVal >>= 1;
            // 1-2i => 1, -1 for 0, 1 and score += for i=0 && score -= for i=1
            score += linesCount[i][j] * factorVal * (1 - (2*i));
        }
    }
    
    return score;
}

