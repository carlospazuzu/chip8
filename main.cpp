#include "game.h"

int main()
{
    Game *game = new Game();
    game->loadGame();
    game->run();

    return 0;
}