#include "defer.hpp"
#include "game.hpp"

int main()
{
    Game::init();
    const Defer _(Game::deinit);

    Game::launch();
}