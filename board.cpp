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
    uint64_t key = this->currentPosition + this->mask + this->bottomMask;
    return key;
}


void BitBoard :: printBitBoard()
{
    uint64_t key = this->currentPosition;
    int shiftFactor;
    
    for (int j = 0; j <= ROWS; j++)
    {
        for (int i = 0; i < COLS; i++)
        {
            shiftFactor = (i * COLS) + (ROWS - j);
            std::cout << this->getBit(key, shiftFactor) << " ";
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
    uint64_t key = this->getKey();
    int shiftFactor;
    
    for (int j = 1; j <= ROWS; j++)
    {
        std::cout << j << "| ";
        for (int i = 0; i < COLS; i++)
        {
            shiftFactor = ((i) * this->directions[0]) + ((ROWS - j) * this->directions[1]);
            if (j <= this->emptyOne[i])
            {
                std::cout << '_' << " | ";
            }
            else if (this->getBit(key, shiftFactor) == 1)
            {
                //since player switch before rendering
                std::cout << this->oppPlayer << " | ";
            }
            else
            {
                std::cout << this->currentPlayer << " | ";
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
        for (int i = 0; i < this->move - 1; i++)
        {
            std::cout << this->moveHistory[i] + 1 << " ";
        }
        std::cout << std::endl;
    }
    return;
}


void BitBoard :: switchPlayers()
{
    if (this->currentPlayer == 'O')
    {
        this->currentPlayer = 'X';
        this->oppPlayer = 'O';
    }
    else
    {
        this->currentPlayer = 'O';
        this->oppPlayer = 'X';
    }
    return;
}


int BitBoard :: getMove()
{
    return this->move;
}


bool BitBoard :: isPlayable(int col)
{
    return (col > -1 && col < COLS && this->emptyOne[col] > 0);
}


void BitBoard :: playMove(int col)
{
    int shiftFactor = ((col) * this->directions[0]) + ((ROWS - this->emptyOne[col]) * this->directions[1]);
    this->mask |= (UINT64_C(1) << shiftFactor);
    this->currentPosition ^= this->mask;
    this->emptyOne[col]--;
    this->moveHistory[this->move - 1] = col;
    this->move++;

    return;
}


void BitBoard :: unPlayMove(int col)
{
    int shiftFactor = ((col) * this->directions[0]) +  ((ROWS - this->emptyOne[col] - 1) * this->directions[1]);
    this->mask ^= (UINT64_C(1) << shiftFactor);
    this->currentPosition ^= (UINT64_C(1) << shiftFactor);
    this->currentPosition ^= this->mask;
    this->emptyOne[col]++;
    this->moveHistory[move - 1] = -1;
    this->move--;

    this->switchPlayers();
    return;
}


int BitBoard :: evalBoard()
{
    int score = 0;

    int linesCount[2][TARGET / 2] = {0};
    int idx;
    int pieceCount, oppPieceCount;
    //for each direction
    for (int i = 0; i < 4; i++)
    {
        uint64_t pos = this->currentPosition;
        uint64_t oppPos = this->currentPosition ^ this->mask;
        idx = 0;
        
        //shift to ensure atleast TARGET / 2 in a direction
        for (int j = 1; j < TARGET / 2; j++)
        {
            pos = pos & (pos >> this->directions[i]);
            oppPos = oppPos & (oppPos >> this->directions[i]);
        }

        pieceCount = countPieces(pos);
        linesCount[0][idx] += pieceCount;
        oppPieceCount = countPieces(oppPos);
        linesCount[1][idx] += oppPieceCount;
        
        //store number of x-in a direction
        while (idx < TARGET / 2)
        {
            pos = pos & (pos >> this->directions[i]);
            pieceCount = countPieces(pos);
            linesCount[0][idx] -= pieceCount;

            oppPos = oppPos & (oppPos >> this->directions[i]);
            oppPieceCount = countPieces(oppPos);
            linesCount[1][idx] -= oppPieceCount;

            linesCount[0][++idx] += pieceCount;
            linesCount[1][idx] += oppPieceCount;
        }
    }
    //now lineCount stores all posssible x in a row
    //generate evaluation formula relative to currentPlayer
    //independent to 'O' or 'X' or maximizer
    score = this->getScore(linesCount);
    score *= -1; // inverted score is needed to be propagated
    
    return score;
}


bool BitBoard :: isWin()
{
    for (int i = 0; i < 4; i++)
    {
        uint64_t pos = this->currentPosition;
        for (int j = 1; j < TARGET; j++)
        {
            pos = pos & (pos >> this->directions[i]);
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
    return ((this->move - 1) == (ROWS * COLS));
}


//PRIVATE FUNCTIONS

void BitBoard :: initVariables()
{
    this->currentPosition = 0;
    this->mask = 0;
    this->bottomMask = 0;
    for (int j = 0; j < COLS; j++)
    {
        this->bottomMask += (UINT64_C(1) << (j * (ROWS+1)));
        this->emptyOne[j] = ROWS;
        
        for (int i = 0; i < ROWS; i++)
        {
            this->moveHistory[j*COLS + i] = -1;
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

