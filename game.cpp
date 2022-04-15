#include <iostream>
#include "game.hpp"

const int MAX_DEPTH = 10;

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
    while (accepted == false || !board.isPlayable(col - 1))
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
        
        if (accepted == false || !board.isPlayable(col - 1))
        {
            std::cout << "Invalid Input!!" << std::endl;
            accepted = false;
        }
    }
    return col - 1;
}

void Game :: manualMove()
{
    int col = manualChoice();
    board.playMove(col);
    return;
}

int bestCol = -1;

//COMMENTED PART IS FOR TRANSPOSTION TABLE IMPLEMENTATION
int Game :: negamax(BitBoard board, int initCol, bool maximizer, int depth, int alpha, int beta)
{
    int score = 0;

    // int hashFlag = HASH_ALPHA;
    // if ((score = board.tt.probeHashElement(depth, alpha, beta)) != INT_MIN)
    // {
    //     return score;
    // }
    
    // score = 0;
    if (board.isWin())
    {
        score -= ROWS*COLS - depth;
    }
    else if (board.isTied())
    {
        score += 0;
    }
    else if (depth == MAX_DEPTH)
    {
        score += board.evalBoard(maximizer);
        if (!maximizer)
        {
            score *= -1;
        }
        // board.tt.recordHashElement(depth, score, bestCol, HASH_EXACT);
    }
    else
    {
        board.switchPlayers();
        for (int i = 0; i <= COLS/2; i++)
        {
            for (int j = COLS/2 - i; j <= COLS/2 + i; j += (i==0) ? 1 : 2*i)
            {
                if (depth == 0)
                {
                    initCol = j;
                }

                if (board.isPlayable(j))
                {
                    board.playMove(j);
                    ++depth;
                    BitBoard newBoard = board;
                    score = -negamax(newBoard, initCol, !maximizer, depth, -beta, -alpha);
                    --depth;
                    board.unPlayMove(j);

                    if (beta <= score)
                    {
                        // board.tt.recordHashElement(depth, score, bestCol, HASH_BETA);
                        return beta;
                    }
                    if (alpha < score)
                    {
                        // hashFlag = HASH_EXACT;
                        alpha = score;
                        if (depth == 0)
                        {
                            bestCol = j;
                        }
                    }
                    if (depth == 0)
                    {
                        std::cout << bestCol << " " << j << " " << score << std::endl;
                    }
                }
            }
        }
        // board.tt.recordHashElement(depth, score, bestCol, hashFlag);
        return alpha;
    }
    return score;
}

void Game :: aiMove()
{
    std::cout << "Computer moving..." << std::endl;
    bestCol = -1;
    BitBoard p_Board = board;
    p_Board.switchPlayers();
    negamax(p_Board, 0, true, 0, -1000, 1000);
    board.playMove(bestCol);
    return;
}

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
    return !(board.isWin() || board.isTied());
}

void Game :: update()
{
    std::cout << "MOVE :: " << board.getMove() << std::endl;
    if (board.getMove() & 1)
    {
        if (gameType == 2)
        {
            aiMove();
        }
        else
        {
            manualMove();
        }
    }
    else
    {
        if (gameType == 0)
        {
            manualMove();
        }
        else
        {
            aiMove();
        }
    }
    board.switchPlayers();

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
    if (board.isWin())
    {
        //since player switches after a move is made.
        std::cout << board.oppPlayer << std::endl;
    }
    else
    {
        std::cout << "TIE" << std::endl;
    }
    return;
}
