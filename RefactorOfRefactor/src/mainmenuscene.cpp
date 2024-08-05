#include "mainmenuscene.hpp"
#include "colors.hpp"
#include "game.hpp"
#include "textbubble.hpp"
#include "texture.hpp"
#include <cstdint>

MainMenuScene::MainMenuScene()
    : sceneLoaded(false)
    , logo({ .Area = initializeLogo() })
    , buttons({
          { retrievePositionOfButton(1), "New Game", []() { Game::EventHandler.changeSceneTo<InGameScene>(); } },
          { retrievePositionOfButton(2), "How to Play", []() { Game::EventHandler.requestEvent({ .type = SDL_EVENT_QUIT }); } },
          { retrievePositionOfButton(3), "Settings", []() { Game::EventHandler.requestEvent({ .type = SDL_EVENT_QUIT }); } },
          { retrievePositionOfButton(4), "Quit", []() { Game::EventHandler.requestEvent({ .type = SDL_EVENT_QUIT }); } },
      })
    , selectedButton(NULL)
    , firstStartSfx(Mix_LoadWAV("assets/audio/main-theme.mp3"))
{
    uint32_t foundFontSize = Game::FontManager.findSuitableFontSize(buttons);
    ASSERT(foundFontSize > 0, "Could not find valid font size for scene");

    Game::TextureLoader.queueTextureLoad({
        .Source = WhereToLoadTextureFrom::FromDisk,
        .Asset = "textures/background.png",
        .Output = &backgroundImage,
    });

    fut::forEach(buttons, [&](auto&& button, const size_t i)
    {
        Game::TextureLoader.queueTextureLoad({
            .Source = WhereToLoadTextureFrom::FromText,
            .Asset = button.getText(),
            .Output = buttons[i].getLabel(),
            .HoldingArea = button.getHoldingArea(),
        });
    });

    Game::TextureLoader.beginTextureLoadProcess();
    Mix_PlayChannel(0, firstStartSfx, 0);
}

void MainMenuScene::deinit() const
{
    Mix_FreeChunk(firstStartSfx);
    SDL_DestroyTexture(backgroundImage.Resource);
    SDL_DestroyTexture(logo.Resource);
    // SDL_DestroyTexture((*buttons[0].getLabel()).Resource);
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
    const uint32_t halfScreenHeight = Game::ScreenHeight / 2;
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
    const uint32_t halfScreenHeight = Game::ScreenHeight / 2;
    const float totalItemSpace = halfScreenHeight * 0.7f;
    const float individualItemSpace = totalItemSpace / BUTTON_COUNT;
    const float totalPaddingSpace = halfScreenHeight - totalItemSpace;
    const float individualPaddingSpace = totalPaddingSpace / (BUTTON_COUNT + 1);

    return SDL_FRect{
        .x = (*logo.Area).x,
        .y = halfScreenHeight + index * individualPaddingSpace + (index - 1) * individualItemSpace,
        .w = (*logo.Area).w,
        .h = individualItemSpace,
    };
}

void MainMenuScene::clicks()
{
    if (selectedButton != NULL)
    {
        (*selectedButton).click();
    }
}
