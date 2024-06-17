#include "defer.hpp"
#include "game.hpp"

int main()
{
    Game::init();
    DEFER(Game::deinit);

    Game::launch();
}