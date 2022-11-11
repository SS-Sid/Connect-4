#include "ai_util.hpp"
#include <iostream>
#include <chrono>


execution_results :: execution_results(int column_chosen, int search_score, int nodes_explored, double execution_time, int depth_checked)
{
    this->col_idx_chosen = column_chosen;
    this->search_score = search_score;
    this->nodes_explored = nodes_explored;
    this->execution_time = execution_time;
    this->depth_checked = depth_checked;
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


execution_results algorithms_ai :: principalVariation(bool isAlphaBeta, bool isPVS, bool isIDDFS)
{
    this->bestCol = -1;
    this->nodeCount = -1;
    int search_score;
    std::chrono::time_point<std::chrono::system_clock> start, end;

    if (isIDDFS == true)
    {
        start = std::chrono::system_clock::now();
        search_score = this->iterative_deepening_util(isAlphaBeta, isPVS);
        end = std::chrono::system_clock::now();
    }
    else
    {
        start = std::chrono::system_clock::now();
        search_score = this->principalVariation_util(this->ai_board, this->maxDepth, isAlphaBeta, isPVS, -1e9, +1e9);
        end = std::chrono::system_clock::now();
    }
    
    std::chrono::duration<double, std::milli> elapsed_seconds = end - start;
    execution_results result(this->bestCol, search_score, this->nodeCount, elapsed_seconds.count(), this->maxDepth);

    return result;
}


// AI MOVE CHOICE ALGORITHM(S)

int algorithms_ai :: principalVariation_util(BitBoard p_board, int depth, bool isAlphaBeta, bool isPVS, int alpha, int beta)
{
    int score = 0;
    this->nodeCount += 1;

    if (p_board.isWin())
    {
        score -= ROWS*COLS + depth - this->maxDepth;
    }
    else if (p_board.isTied())
    {
        score += 0;
    }
    else if (depth == 0)
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
                    
                    BitBoard newBoard = p_board;
                    if (foundPV == true && isPVS == true)
                    {
                        score = - this->principalVariation_util(newBoard, depth - 1, isAlphaBeta, isPVS, -alpha - 1, -alpha);

                        if ((alpha < score) && (beta > score))
                        {
                            score = - this->principalVariation_util(newBoard, depth - 1, isAlphaBeta, isPVS, -beta, -alpha);
                        }
                    }
                    else
                    {
                        score = - this->principalVariation_util(newBoard, depth - 1, isAlphaBeta, isPVS, -beta, -alpha);
                    }

                    p_board.unPlayMove(j);

                    if (isAlphaBeta == true && beta <= score)
                    {
                        return beta;
                    }
                    if (alpha < score)
                    {
                        alpha = score;
                        foundPV = true;

                        if (depth == this->maxDepth)
                        {
                            this->bestCol = j;
                        }
                    }

                    if (depth == this->maxDepth)
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


int algorithms_ai :: iterative_deepening_util(bool isAlphaBeta, bool isPVS)
{
    std::chrono::time_point<std::chrono::system_clock> start, curr;
    std::chrono::duration<double, std::milli> elapsed_seconds;
    int search_score;

    start = std::chrono::system_clock::now();
    for (int iter_max_depth = 0; ; iter_max_depth++)
    {
        curr = std::chrono::system_clock::now();
        elapsed_seconds = curr - start;
        std::cout << "\t\t" << elapsed_seconds.count() << std::endl;
        if (elapsed_seconds.count() >= TIMEOUT_VAL || ai_board.getMove() + this->maxDepth == ROWS * COLS)
        {
            break;
        }

        this->maxDepth = iter_max_depth;
        search_score = this->principalVariation_util(this->ai_board, this->maxDepth, isAlphaBeta, isPVS, -1e9, +1e9);
    }

    return search_score;
}