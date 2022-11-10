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


// AI MOVE CHOICE ALGORITHM(S)

int Game :: negamax(BitBoard p_board, int initCol, int depth)
{
    int score = 0;
    this->nodeCount += 1;

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
        int best_score = -1000; // -INF
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
                    score = - this->negamax(newBoard, initCol, depth);
                    
                    --depth;
                    p_board.unPlayMove(j);

                    if (best_score < score)
                    {
                        best_score = score;
                        
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
        return best_score;
    }
    return score;
}


int Game :: alphaBeta(BitBoard p_board, int initCol, int depth, int alpha, int beta)
{
    int score = 0;
    this->nodeCount += 1;

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
                    score = - this->alphaBeta(newBoard, initCol, depth, -beta, -alpha);
                    
                    --depth;
                    p_board.unPlayMove(j);

                    if (beta <= score)
                    {
                        return beta; // fail-hard beta-cutoff
                    }
                    if (alpha < score)
                    {
                        alpha = score; // alpha acts like max of minimax
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


int Game :: principalVariation(BitBoard p_board, int initCol, int depth, int alpha, int beta)
{
    int score = 0;
    this->nodeCount += 1;

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
        bool foundPV = false;
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
                    if (foundPV == true)
                    {
                        score = - this->principalVariation(newBoard, initCol, depth, -alpha - 1, -alpha);

                        if ((alpha < score) && (beta > score))
                        {
                            score = - this->principalVariation(newBoard, initCol, depth, -beta, -alpha);
                        }
                    }
                    else
                    {
                        score = - this->principalVariation(newBoard, initCol, depth, -beta, -alpha);
                    }

                    --depth;
                    p_board.unPlayMove(j);

                    if (beta <= score)
                    {
                        return beta;
                    }
                    if (alpha < score)
                    {
                        alpha = score;
                        foundPV = true;

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
    this->nodeCount = -1;
    BitBoard p_Board = this->board;

    p_Board.switchPlayers();
    std::chrono::time_point<std::chrono::system_clock> start, end;
    start = std::chrono::system_clock::now();
    // this->negamax(p_Board, 0, 0);
    // this->alphaBeta(p_Board, 0, 0, -1000, 1000); // -INF, +INF
    this->principalVariation(p_Board, 0, 0, -1000, 1000); // -INF, +INF
    end = std::chrono::system_clock::now();
    
    this->board.playMove(this->bestCol);

    std::cout << "\tNODE COUNT::" << this->nodeCount << std::endl;
    std::chrono::duration<double> elapsed_seconds = end - start;
    std::cout << "\telapsed time: " << elapsed_seconds.count() << "s\n";
    
    return;
}

