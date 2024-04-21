#include "scene.hpp"
#include "game.hpp"

void MainMenuScene::redraw() const
{
    const auto renderer = Game::renderer;

    int max_number = 255;
    int minimum_number = 0;
    
    int r = rand() % (max_number + 1 - minimum_number) + minimum_number;
    int g = rand() % (max_number + 1 - minimum_number) + minimum_number;
    int b = rand() % (max_number + 1 - minimum_number) + minimum_number;

    SDL_SetRenderDrawColor(renderer, r, g, b, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
}

void InGameScene::redraw() const
{
    const auto renderer = Game::renderer;

    int max_number = 255;
    int minimum_number = 0;
    
    int r = rand() % (max_number + 1 - minimum_number) + minimum_number;
    int g = rand() % (max_number + 1 - minimum_number) + minimum_number;
    int b = rand() % (max_number + 1 - minimum_number) + minimum_number;

    SDL_SetRenderDrawColor(renderer, r, g, b, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
}