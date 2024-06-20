#include "scenes.hpp"
#include "colors.hpp"
#include "game.hpp"
#include "textbubble.hpp"
#include "texture.hpp"
#include <cstdint>
#include <string>

MainMenuScene::MainMenuScene()
    : halfScreenHeight(Game::ScreenHeight / 2)
    , logo({ .Area = initializeLogo() })
    , newGame(retrievePositionOfButton(1))
    , howToPlay(retrievePositionOfButton(2))
    , settings(retrievePositionOfButton(3))
    , quit(retrievePositionOfButton(4))
{
    Game::TextureLoader.queueTextureLoadFromFile({ .Path = "textures/background.png", .Output = &backgroundImage.Resource });
    Game::TextureLoader.beginTextureLoadProcess();
}

MainMenuScene::~MainMenuScene()
{
    SDL_DestroyTexture(logo.Resource);
    SDL_DestroyTexture(backgroundImage.Resource);
}

void MainMenuScene::redraw() const
{
    const auto renderer = Game::Renderer;
    ext::changeDrawColorTo(renderer, col::BLACK);
    SDL_RenderClear(renderer);

    ext::drawTextureRegion(renderer, backgroundImage);
    ext::drawTextureRegion(renderer, logo);

    newGame.draw();
    howToPlay.draw();
    settings.draw();
    quit.draw();

    SDL_RenderPresent(renderer);
}

SDL_FRect MainMenuScene::initializeLogo()
{
    const auto fittingLogoTexture = Game::TextureLoader.findTextureThatFitsIntoArea(halfScreenHeight, halfScreenHeight, "logo");
    Game::TextureLoader.queueTextureLoadFromFile({ .Path = fittingLogoTexture.Path, .Output = &logo.Resource });

    return SDL_FRect{
        .x = Game::ScreenWidth / 2.0f - fittingLogoTexture.Width / 2.0f,
        .y = (halfScreenHeight - fittingLogoTexture.Height) / 2.0f,
        .w = static_cast<float>(fittingLogoTexture.Width),
        .h = static_cast<float>(fittingLogoTexture.Height),
    };
}

SDL_FRect MainMenuScene::retrievePositionOfButton(const uint32_t index) const
{
    const auto totalItemSpace = halfScreenHeight * 0.7f;
    const auto individualItemSpace = totalItemSpace / buttonCount;
    const auto totalPaddingSpace = halfScreenHeight - totalItemSpace;
    const auto individualPaddingSpace = totalPaddingSpace / (buttonCount + 1);

    return SDL_FRect{
        .x = (*logo.Area).x,
        .y = halfScreenHeight + index * individualPaddingSpace + (index - 1) * individualItemSpace,
        .w = (*logo.Area).w,
        .h = individualItemSpace,
    };
}

void InGameScene::init()
{
}

void InGameScene::deinit()
{
}

void InGameScene::redraw() const
{
    const auto renderer = Game::Renderer;

    int max_number = 255;
    int minimum_number = 0;

    int r = rand() % (max_number + 1 - minimum_number) + minimum_number;
    int g = rand() % (max_number + 1 - minimum_number) + minimum_number;
    int b = rand() % (max_number + 1 - minimum_number) + minimum_number;

    SDL_SetRenderDrawColor(renderer, r, g, b, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
}