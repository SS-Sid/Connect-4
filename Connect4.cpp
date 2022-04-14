#include "game.h"

int main()
{
    Game game;

    while (game.isGameRunning())
    {
        game.update();
        game.render();
    }
    
    game.endGame();

    return 0;
}
