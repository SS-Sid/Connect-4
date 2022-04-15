#ifndef GAME_H
#define GAME_H

#include "board.hpp"

class Game
{
private:
    // PRIVATE VARIABLES
    int gameType;
    BitBoard board;

    // PRIVATE FUNCTIONS
    void initGameType();
    int manualChoice();
    void manualMove();
    int negamax(BitBoard board, int initCol, bool maximizer, int depth, int alpha, int beta);
    void aiMove();

public:
    //PUBLIC VARIABLES

    // PUBLIC FUNCTIONS
    Game();
    ~Game();

    bool isGameRunning();
    void update();
    void render();
    void endGame();

};

#endif
