#include <iostream>
#include "game.hpp"

// PUBLIC FUNCTIONS

Game :: Game()
{
    std::cout << "WELCOME TO CONNECT-4" << std::endl;
    this->initGameType();
    this->render();
}


Game :: ~Game()
{
    //
}


bool Game :: isGameRunning()
{
    return !(this->board.isWin() || this->board.isTied());
}


void Game :: update()
{
    std::cout << "MOVE :: " << this->board.getMove() << std::endl;
    if (this->board.getMove() & 1)
    {
        if (this->gameType == 2)
        {
            this->aiMove();
        }
        else
        {
            this->manualMove();
        }
    }
    else
    {
        if (this->gameType == 0)
        {
            this->manualMove();
        }
        else
        {
            this->aiMove();
        }
    }
    this->board.switchPlayers();

    return;
}


void Game :: render()
{
    this->board.printBitBoard();
    this->board.printBoard();
    this->board.printMoveHistory();
    return;
}


void Game :: endGame()
{
    std::cout << "GAME OVER" << std::endl;
    if (this->board.isWin())
    {
        //since player switches after a move is made.
        std::cout << this->board.oppPlayer << std::endl;
    }
    else
    {
        std::cout << "TIE" << std::endl;
    }
    return;
}


// PRIVATE FUNCTIONS


void Game :: initGameType()
{
    bool inputAccepted = false;
    int num;
    while (inputAccepted == false)
    {
        std::cout << "CHOICES:: 0 for PvP, 1 for PvAI, 2 for AIvAI\nENTER CHOICE:: ";
        std::cin >> num;
        for (int i = 0; i < 3; i++)
        {
            if (i == num)
            {
                inputAccepted = true;
            }
        }
        if (inputAccepted == false)
        {
            std::cout << "Invalid Input!!" << std::endl;
        }
    }
    
    this->gameType = num;
    return;
}


int Game :: manualChoice()
{
    bool accepted = false;
    int col;
    while (accepted == false || !this->board.isPlayable(col - 1))
    {
        std::cout << "Select Column Number :: ";
        std::cin >> col;
        for (int i = 1; i <= COLS; i++)
        {
            if (i == col)
            {
                accepted = true;
            }
        }
        
        if (accepted == false || !this->board.isPlayable(col - 1))
        {
            std::cout << "Invalid Input!!" << std::endl;
            accepted = false;
        }
    }
    return col - 1;
}


void Game :: manualMove()
{
    int col = this->manualChoice();
    this->board.playMove(col);
    return;
}
int Game :: negamax(BitBoard p_board, int initCol, int depth, int alpha, int beta)
{
    int score = 0;

    if (p_board.isWin())
    {
        score -= ROWS*COLS - depth;
    }
    else if (p_board.isTied())
    {
        score += 0;
    }
    else if (depth == MAX_DEPTH)
    {
        score += p_board.evalBoard();
    }
    else
    {
        p_board.switchPlayers();
        for (int i = 0; i <= COLS/2; i++)
        {
            for (int j = COLS/2 - i; j <= COLS/2 + i; j += (i==0) ? 1 : 2*i)
            {
                if (depth == 0)
                {
                    initCol = j;
                }

                if (p_board.isPlayable(j))
                {
                    p_board.playMove(j);
                    ++depth;
                    BitBoard newBoard = p_board;
                    score = - this->negamax(newBoard, initCol, depth, -beta, -alpha);
                    --depth;
                    p_board.unPlayMove(j);

                    if (beta <= score)
                    {
                        return beta;
                    }
                    if (alpha < score)
                    {
                        alpha = score;
                        if (depth == 0)
                        {
                            this->bestCol = j;
                        }
                    }
                    if (depth == 0)
                    {
                        std::cout << this->bestCol + 1 << " " << j + 1 << " " << score << std::endl;
                    }
                }
            }
        }
        return alpha;
    }
    return score;
}


void Game :: aiMove()
{
    std::cout << "Computer moving..." << std::endl;
    this->bestCol = -1;
    BitBoard p_Board = this->board;
    p_Board.switchPlayers();
    this->negamax(p_Board, 0, 0, -1000, 1000);
    this->board.playMove(this->bestCol);
    return;
}

