#ifndef GAME_H
#define GAME_H

#include "board.hpp"

const int MAX_DEPTH = 10;

class Game
{
public:
    //PUBLIC VARIABLES

    // PUBLIC FUNCTIONS
    Game();
    ~Game();

    bool isGameRunning();
    void update();
    void render();
    void endGame();


private:
    // PRIVATE VARIABLES
    int gameType;
    BitBoard board;
    int bestCol;

    // PRIVATE FUNCTIONS
    void initGameType();
    int manualChoice();
    void manualMove();
    int negamax(BitBoard board, int initCol, bool maximizer, int depth, int alpha, int beta);
    void aiMove();
};

#endif
