#ifndef AI_UTIL_H
#define AI_UTIL_H

#include "board.hpp"


class algorithms_ai
{
public:
    algorithms_ai(BitBoard p_board, int depth);
    ~algorithms_ai();

    int negamax();
    int alphaBeta();
    int principalVariation();

    int getBestCol();

private:
    BitBoard ai_board;
    int maxDepth;

    int bestCol;
    int nodeCount;

    
    int negamax_util(BitBoard p_board, int depth);
    int alphaBeta_util(BitBoard p_board, int depth, int alpha, int beta);
    int principalVariation_util(BitBoard p_board, int depth, int alpha, int beta);
};


#endif