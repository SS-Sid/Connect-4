#include "ai_util.hpp"

#include <iostream>
#include <chrono>


algorithms_ai :: algorithms_ai(BitBoard p_board, int depth)
{
    this->ai_board = p_board;
    this->maxDepth = depth;
}


int algorithms_ai :: negamax()
{
    this->bestCol = -1;
    this->nodeCount = -1;
    
    this->negamax_util(this->ai_board, this->maxDepth);

    return this->bestCol;
}


int algorithms_ai :: alphaBeta()
{
    this->bestCol = -1;
    this->nodeCount = -1;
    
    this->alphaBeta_util(this->ai_board, this->maxDepth, -1e9, +1e9);

    return this->bestCol;
}


int algorithms_ai :: principalVariation()
{
    this->bestCol = -1;
    this->nodeCount = -1;
    
    this->principalVariation_util(this->ai_board, this->maxDepth, -1e9, +1e9);

    return this->bestCol;
}






// AI MOVE CHOICE ALGORITHM(S)

int algorithms_ai :: negamax_util(BitBoard p_board, int depth)
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
    else if (depth == maxDepth)
    {
        score += p_board.evalBoard();
    }
    else
    {
        int best_score = -1e9; // -INF
        p_board.switchPlayers();
        for (int i = 0; i <= COLS/2; i++)
        {
            for (int j = COLS/2 - i; j <= COLS/2 + i; j += (i==0) ? 1 : 2*i)
            {
                // if (depth == 0)
                // {
                //     this->bestCol = -1;
                //     this->nodeCount = 0;
                // }

                if (p_board.isPlayable(j))
                {
                    p_board.playMove(j);
                    ++depth;
                    
                    BitBoard newBoard = p_board;
                    score = - this->negamax_util(newBoard, depth);
                    
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


int algorithms_ai :: alphaBeta_util(BitBoard p_board, int depth, int alpha, int beta)
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
    else if (depth == maxDepth)
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
                // if (depth == 0)
                // {
                //     this->bestCol = -1;
                //     this->nodeCount = 0;
                // }

                if (p_board.isPlayable(j))
                {
                    p_board.playMove(j);
                    ++depth;
                    
                    BitBoard newBoard = p_board;
                    score = - this->alphaBeta_util(newBoard, depth, -beta, -alpha);
                    
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


int algorithms_ai :: principalVariation_util(BitBoard p_board, int depth, int alpha, int beta)
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
    else if (depth == maxDepth)
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
                // if (depth == 0)
                // {
                //     this->bestCol = -1;
                //     this->nodeCount = 0;
                // }

                if (p_board.isPlayable(j))
                {
                    p_board.playMove(j);
                    ++depth;
                    
                    BitBoard newBoard = p_board;
                    if (foundPV == true)
                    {
                        score = - this->principalVariation_util(newBoard, depth, -alpha - 1, -alpha);

                        if ((alpha < score) && (beta > score))
                        {
                            score = - this->principalVariation_util(newBoard, depth, -beta, -alpha);
                        }
                    }
                    else
                    {
                        score = - this->principalVariation_util(newBoard, depth, -beta, -alpha);
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

