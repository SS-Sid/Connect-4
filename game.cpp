#include <iostream>
#include <cstdio>
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

    for (int i = 0; i < 50; i++)
    {
        std::cout << "-";
    }
    std::cout << std::endl;
    
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

    this->result_fp = fopen("./results.csv", "w");
    fclose(this->result_fp);
    
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
    BitBoard p_board = this->board;
    p_board.switchPlayers();

    algorithms_ai aiUtil(p_board, 10); // 10 = MAX_DEPTH
    execution_results result = aiUtil.principalVariation(true, true, false);

    std::cout << "\t\t  column_chosen:: " << result.col_idx_chosen + 1 << std::endl;
    std::cout << "\t\t   search_score:: " << result.search_score << std::endl;
    std::cout << "\t\t nodes_explored:: " << result.nodes_explored << std::endl;
    std::cout << "\t\t execution_time:: " << result.execution_time << std::endl;
    std::cout << "\t\t  depth_checked:: " << result.depth_checked << std::endl;

    this->result_fp = fopen("./results.csv", "a");
    fprintf(this->result_fp, "%d,%d,%d,%lf,%d\n", result.col_idx_chosen, result.search_score, result.nodes_explored, result.execution_time, result.depth_checked);
    fclose(this->result_fp);

    return result.col_idx_chosen;
}

void Game :: aiMove()
{
    int col = this->aiChoice();
    this->board.playMove(col);
    return;
}