#ifndef AI_UTIL_H
#define AI_UTIL_H

#include "board.hpp"


class algorithms_ai
{
public:
    algorithms_ai(BitBoard p_board, int depth);
    ~algorithms_ai();

    int principalVariation(bool isAlphaBeta, bool isPVS);

private:
    BitBoard ai_board;
    int maxDepth;

    int bestCol;
    int nodeCount;

    
    int principalVariation_util(BitBoard p_board, int depth, bool isAlphaBeta, bool isPVS, int alpha, int beta);
};


#endif