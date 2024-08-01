#include "scenes.hpp"
#include "colors.hpp"
#include "game.hpp"
#include "textbubble.hpp"
#include "texture.hpp"
#include <cstdint>
#include <string>

MainMenuScene::MainMenuScene()
    : halfScreenHeight(Game::ScreenHeight / 2)
    , sceneLoaded(false)
    , logo({ .Area = initializeLogo() })
    , buttons({
          { retrievePositionOfButton(1), "New Game" },
          { retrievePositionOfButton(2), "How to Play" },
          { retrievePositionOfButton(3), "Settings" },
          { retrievePositionOfButton(4), "Quit" },
      })
    , selectedButton(NULL)
    , firstStartSfx(Mix_LoadWAV("assets/audio/main-theme.mp3"))
{
    uint32_t foundFontSize = Game::FontManager.findSuitableFontSize(buttons);
    ASSERT(foundFontSize > 0, "Could not find valid font size for scene");
    LOG("FontSize: {}", foundFontSize);

    Game::TextureLoader.queueTextureLoad({
        .Source = WhereToLoadTextureFrom::FromDisk,
        .Asset = "textures/background.png",
        .Output = &backgroundImage,
    });

    fut::forEach(buttons, [&](const auto& /*button*/, const size_t i)
    {
        Game::TextureLoader.queueTextureLoad({
            .Source = WhereToLoadTextureFrom::FromText,
            .Asset = buttons[i].getText(),
            .Output = buttons[i].getLabel(),
            .HoldingArea = buttons[i].getHoldingArea(),
        });
    });

    // Game::TextureLoader.queueTextureLoad({
    //     .Source = WhereToLoadTextureFrom::FromText,
    //     .Asset = buttons[0].getText(),
    //     .Output = buttons[0].getLabel(),
    //     .HoldingArea = buttons[0].getHoldingArea(),
    // });

    // Game::TextureLoader.queueTextureLoad({
    //     .Source = WhereToLoadTextureFrom::FromText,
    //     .Asset = buttons[1].getText(),
    //     .Output = buttons[1].getLabel(),
    //     .HoldingArea = buttons[1].getHoldingArea(),
    // });

    // Game::TextureLoader.queueTextureLoad({
    //     .Source = WhereToLoadTextureFrom::FromText,
    //     .Asset = buttons[2].getText(),
    //     .Output = buttons[2].getLabel(),
    //     .HoldingArea = buttons[2].getHoldingArea(),
    // });

    // Game::TextureLoader.queueTextureLoad({
    //     .Source = WhereToLoadTextureFrom::FromText,
    //     .Asset = buttons[3].getText(),
    //     .Output = buttons[3].getLabel(),
    //     .HoldingArea = buttons[3].getHoldingArea(),
    // });

    Game::TextureLoader.beginTextureLoadProcess();
    Mix_PlayChannel(0, firstStartSfx, 0);
}

void MainMenuScene::deinit() const
{
    Mix_FreeChunk(firstStartSfx);
    SDL_DestroyTexture(backgroundImage.Resource);
    SDL_DestroyTexture(logo.Resource);
}

void MainMenuScene::redraw() const
{
    const auto renderer = Game::Renderer;
    ext::changeDrawColorTo(renderer, col::BLACK);
    SDL_RenderClear(renderer);

    if (sceneLoaded)
    {
        ext::drawTextureRegion(renderer, backgroundImage);
        ext::drawTextureRegion(renderer, logo);

        fut::forEach(buttons, [](const auto& button, const size_t /*i*/) {
            button.draw();
        });
    }

    SDL_RenderPresent(renderer);
}

void MainMenuScene::intersects(SDL_FPoint&& location)
{
    TextBubble* newlySelectedButton = NULL;

    fut::forEach(buttons, [&](const auto& button, const size_t i)
    {
        if (button.isInsideItsHitbox(location))
        {
            newlySelectedButton = &buttons[i];
        }
    });

    if (selectedButton != newlySelectedButton)
    {
        if (selectedButton != NULL)
        {
            (*selectedButton).disableHover();
        }

        selectedButton = newlySelectedButton;

        if (selectedButton != NULL)
        {
            (*selectedButton).enableHover();
        }

        Game::EventHandler.invalidate();
    }
}

void MainMenuScene::enable()
{
    sceneLoaded = true;
}

SDL_FRect MainMenuScene::initializeLogo()
{
    const auto fittingLogoTexture = Game::TextureLoader.findTextureThatFitsIntoArea(halfScreenHeight, halfScreenHeight, "logo");
    Game::TextureLoader.queueTextureLoad({
        .Source = WhereToLoadTextureFrom::FromDisk,
        .Asset = fittingLogoTexture.Path,
        .Output = &logo,
    });

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
    const auto individualItemSpace = totalItemSpace / BUTTON_COUNT;
    const auto totalPaddingSpace = halfScreenHeight - totalItemSpace;
    const auto individualPaddingSpace = totalPaddingSpace / (BUTTON_COUNT + 1);

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

void InGameScene::deinit() const
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

void InGameScene::intersects(SDL_FPoint&& /*location*/)
{
}