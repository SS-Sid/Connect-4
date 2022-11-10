#ifndef GAME_H
#define GAME_H

#include "board.hpp"
#include "ai_util.hpp"

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

    // PRIVATE FUNCTIONS
    void initGameType();

    int manualChoice();
    void manualMove();

    int aiChoice();
    void aiMove();
};

#endif
