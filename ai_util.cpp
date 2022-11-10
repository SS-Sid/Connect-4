#include "ai_util.hpp"
#include <iostream>
#include <chrono>


execution_results :: execution_results(int column_chosen, int search_score, int nodes_explored, double execution_time)
{
    this->col_idx_chosen = column_chosen;
    this->search_score = search_score;
    this->nodes_explored = nodes_explored;
    this->execution_time = execution_time;
}


algorithms_ai :: algorithms_ai(BitBoard p_board, int depth)
{
    this->ai_board = p_board;
    this->maxDepth = depth;
}


algorithms_ai :: ~algorithms_ai()
{
    //
}


execution_results algorithms_ai :: principalVariation(bool isAlphaBeta, bool isPVS)
{
    this->bestCol = -1;
    this->nodeCount = -1;
    
    std::chrono::time_point<std::chrono::system_clock> start, end;

    start = std::chrono::system_clock::now();
    int search_score = this->principalVariation_util(this->ai_board, 0, isAlphaBeta, isPVS, -1e9, +1e9);
    end = std::chrono::system_clock::now();
    
    std::chrono::duration<double, std::milli> elapsed_seconds = end - start;
    execution_results result(this->bestCol, search_score, this->nodeCount, elapsed_seconds.count());

    return result;
}


// AI MOVE CHOICE ALGORITHM(S)

int algorithms_ai :: principalVariation_util(BitBoard p_board, int depth, bool isAlphaBeta, bool isPVS, int alpha, int beta)
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
        if (isAlphaBeta == false)
        {
            alpha = -1e9;
        }
        bool foundPV = false;
        p_board.switchPlayers();
        for (int i = 0; i <= COLS/2; i++)
        {
            for (int j = COLS/2 - i; j <= COLS/2 + i; j += (i==0) ? 1 : 2*i)
            {
                if (p_board.isPlayable(j))
                {
                    p_board.playMove(j);
                    ++depth;
                    
                    BitBoard newBoard = p_board;
                    if (foundPV == true && isPVS == true)
                    {
                        score = - this->principalVariation_util(newBoard, depth, isAlphaBeta, isPVS, -alpha - 1, -alpha);

                        if ((alpha < score) && (beta > score))
                        {
                            score = - this->principalVariation_util(newBoard, depth, isAlphaBeta, isPVS, -beta, -alpha);
                        }
                    }
                    else
                    {
                        score = - this->principalVariation_util(newBoard, depth, isAlphaBeta, isPVS, -beta, -alpha);
                    }

                    --depth;
                    p_board.unPlayMove(j);

                    if (isAlphaBeta == true && beta <= score)
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

