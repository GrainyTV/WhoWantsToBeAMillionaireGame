#include "scenes.hpp"
// #include "SDL3_image/SDL_image.h"
#include "assert.hpp"
#include "game.hpp"
#include "textbubble.hpp"
#include "texture.hpp"
#include <algorithm>
#include <cstdint>
#include <filesystem>
#include <regex>
#include <string>
#include <vector>
// #include <format>
// #include <string>

static constexpr SDL_Color BACKGROUND{ .r = 0, .g = 0, .b = 0, .a = 255 };

MainMenuScene::MainMenuScene()
    : halfScreenHeight(Game::ScreenHeight / 2)
    , logo({
          .Resource = NULL,
          .Area = (*this).initializeLogo(),
      })
    , newGame((*this).retrievePositionOfButton(1))
    , howToPlay((*this).retrievePositionOfButton(2))
    , settings((*this).retrievePositionOfButton(3))
    , quit((*this).retrievePositionOfButton(4))
{
    // const auto halfScreenHeight = Game::ScreenHeight / 2;
    // const auto fittingLogoTexture = Texture::findTextureThatFitsIntoArea(halfScreenHeight, halfScreenHeight, "logo");

    // logo.Area.x = Game::ScreenWidth / 2.0f - fittingLogoTexture.Width / 2.0f;
    // logo.Area.y = (halfScreenHeight - fittingLogoTexture.Height) / 2.0f;
    // logo.Area.w = fittingLogoTexture.Width;
    // logo.Area.h = fittingLogoTexture.Height;

    /*const auto totalItemSpace = halfScreenHeight * 0.7f;
    const auto individualItemSpace = totalItemSpace / buttonCount;
    const auto totalPaddingSpace = halfScreenHeight - totalItemSpace;
    const auto individualPaddingSpace = totalPaddingSpace / (buttonCount + 1);*/

    /*SDL_FRect tbTemplate{
        .x = logo.Area.x,
        .y = -1,
        .w = logo.Area.w,
        .h = individualItemSpace,
    };*/

    /*button1.x = logo.Area.x;
    button2.x = logo.Area.x;
    button3.x = logo.Area.x;
    button4.x = logo.Area.x;

    button1.w = logo.Area.w;
    button2.w = logo.Area.w;
    button3.w = logo.Area.w;
    button4.w = logo.Area.w;*/

    /*tbTemplate.y = halfScreenHeight + 1 * individualPaddingSpace + 0 * individualItemSpace;
    newGame = TextBubble{ tbTemplate };

    tbTemplate.y = halfScreenHeight + 2 * individualPaddingSpace + 1 * individualItemSpace;
    howToPlay = TextBubble{ tbTemplate };

    tbTemplate.y = halfScreenHeight + 3 * individualPaddingSpace + 2 * individualItemSpace;
    settings = TextBubble{ tbTemplate };

    tbTemplate.y = halfScreenHeight + 4 * individualPaddingSpace + 3 * individualItemSpace;
    quit = TextBubble{ tbTemplate };*/

    /*button1.y = halfScreenHeight + 1 * individualPaddingSpace + 0 * individualItemSpace;
    button2.y = halfScreenHeight + 2 * individualPaddingSpace + 1 * individualItemSpace;
    button3.y = halfScreenHeight + 3 * individualPaddingSpace + 2 * individualItemSpace;
    button4.y = halfScreenHeight + 4 * individualPaddingSpace + 3 * individualItemSpace;

    button1.h = individualItemSpace;
    button2.h = individualItemSpace;
    button3.h = individualItemSpace;
    button4.h = individualItemSpace;*/

    Game::TextureLoader->queueTextureLoadFromFile("logo", &logo.Resource);
    Game::TextureLoader->queueTextureLoadFromFile("background", &backgroundImage);
    Game::TextureLoader->allTexturesLoaded();
}

MainMenuScene::~MainMenuScene()
{
    SDL_DestroyTexture(logo.Resource);
    SDL_DestroyTexture(backgroundImage);
}

void MainMenuScene::redraw() const
{
    const auto renderer = Game::Renderer;
    SDL_SetRenderDrawColor(renderer, BACKGROUND.r, BACKGROUND.g, BACKGROUND.b, BACKGROUND.a);
    SDL_RenderClear(renderer);

    if (backgroundImage != NULL)
    {
        const auto textureFilter = SDL_SetTextureScaleMode(backgroundImage, SDL_SCALEMODE_BEST);
        ASSERT(textureFilter == 0, "Failed to change texture filtering mode ({})", SDL_GetError());
        const auto render = SDL_RenderTexture(renderer, backgroundImage, NULL, NULL);
        ASSERT(render == 0, "Failed to render background ({})", SDL_GetError());
    }

    if (logo.Resource != NULL)
    {
        const auto textureFilter = SDL_SetTextureScaleMode(logo.Resource, SDL_SCALEMODE_BEST);
        ASSERT(textureFilter == 0, "Failed to change texture filtering mode ({})", SDL_GetError());
        const auto render = SDL_RenderTexture(renderer, logo.Resource, NULL, &logo.Area);
        ASSERT(render == 0, "Failed to render logo ({})", SDL_GetError());
    }

    newGame.render();
    howToPlay.render();
    settings.render();
    quit.render();

    SDL_RenderPresent(renderer);
}

SDL_FRect MainMenuScene::initializeLogo()
{
    const auto fittingLogoTexture = Texture::findTextureThatFitsIntoArea(halfScreenHeight, halfScreenHeight, "logo");

    return SDL_FRect{
        .x = Game::ScreenWidth / 2.0f - fittingLogoTexture.Width / 2.0f,
        .y = (halfScreenHeight - fittingLogoTexture.Height) / 2.0f,
        .w = static_cast<float>(fittingLogoTexture.Width),
        .h = static_cast<float>(fittingLogoTexture.Height),
    };
}

SDL_FRect MainMenuScene::retrievePositionOfButton(const uint32_t index) const
{
    const auto halfScreenHeight = Game::ScreenHeight / 2;
    const auto totalItemSpace = halfScreenHeight * 0.7f;
    const auto individualItemSpace = totalItemSpace / buttonCount;
    const auto totalPaddingSpace = halfScreenHeight - totalItemSpace;
    const auto individualPaddingSpace = totalPaddingSpace / (buttonCount + 1);

    return SDL_FRect{
        .x = logo.Area.x,
        .y = halfScreenHeight + index * individualPaddingSpace + (index - 1) * individualItemSpace,
        .w = logo.Area.w,
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