#ifndef AI_UTIL_H
#define AI_UTIL_H

#include "board.hpp"

const double TIMEOUT_VAL = 10000; //ms (10s)

class execution_results
{
public:
    int col_idx_chosen;
    int search_score;

    int nodes_explored;
    double execution_time; //ms

    int depth_checked;

    execution_results(int column_chosen, int search_score, int nodes_explored, double execution_time, int depth_checked);
};


class algorithms_ai
{
public:
    algorithms_ai(BitBoard p_board, int depth);
    ~algorithms_ai();

    execution_results principalVariation(bool isAlphaBeta, bool isPVS, bool isIDDFS);

private:
    BitBoard ai_board;
    int maxDepth;

    int bestCol;
    int nodeCount;

    int principalVariation_util(BitBoard p_board, int depth, bool isAlphaBeta, bool isPVS, int alpha, int beta);
    int iterative_deepening_util(bool isAlphaBeta, bool isPVS);
};


#endif