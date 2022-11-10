#include <iostream>
#include <chrono>
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


int Game :: aiChoice()
{
    algorithms_ai aiUtil(this->board, 10); // 10 = MAX_DEPTH
    
    int col = aiUtil.negamax();
    // aiUtil.alphaBeta();
    // aiUtil.principalVariation();

    return col;
}

void Game :: aiMove()
{
    int col = this->aiChoice();
    this->board.playMove(col);
    return;
}