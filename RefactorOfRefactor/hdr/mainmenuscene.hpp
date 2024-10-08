// #include "mainmenuscene.hpp"
// //#include "colors.hpp"
// //#include "game.hpp"
// #include "textbubble.hpp"
// //#include "texture.hpp"
// #include <cstdint>
// #include "globals.hpp"
// #include "extensions.hpp"
// #include "functionals.hpp"
// #include "texture.hpp"
// //#include "fontmanager.hpp"

// #include Color;
// #include FontManager;
// #include Texture;

// MainMenuScene::MainMenuScene()
//     : sceneLoaded(false)
//     , logo({ .Area = initializeLogo() })
//     , buttons({
//           { retrievePositionOfButton(1), "New Game", []() { /*event::changeSceneTo<InGameScene>();*/ } },
//           { retrievePositionOfButton(2), "How to Play", []() { utility::requestEvent({ .type = SDL_EVENT_QUIT }); } },
//           { retrievePositionOfButton(3), "Settings", []() { utility::requestEvent({ .type = SDL_EVENT_QUIT }); } },
//           { retrievePositionOfButton(4), "Quit", []() { utility::requestEvent({ .type = SDL_EVENT_QUIT }); } },
//       })
//     , selectedButton(NULL)
//     , firstStartSfx(Mix_LoadWAV("assets/audio/main-theme.mp3"))
// {
//     uint32_t foundFontSize = fontmanager::findSuitableFontSize(buttons);
//     ASSERT(foundFontSize > 0, "Could not find valid font size for scene");

//     MultiSizeTexture background = texture::findTextureThatFitsIntoArea(
//         globals::Properties.value().ScreenWidth, //Game::ScreenWidth,
//         globals::Properties.value().ScreenHeight, //Game::ScreenHeight,
//         "background"
//     );
    
//     texture::queueTextureLoad({
//         .Source = WhereToLoadTextureFrom::FromDisk,
//         .Asset = background.Path,
//         .Output = &backgroundImage,
//     });

//     fut::forEach(buttons, [&](auto&& button, const size_t i)
//     {
//         texture::queueTextureLoad({
//             .Source = WhereToLoadTextureFrom::FromText,
//             .Asset = button.getText(),
//             .Output = buttons[i].getLabel(),
//             .HoldingArea = button.getHoldingArea(),
//         });
//     });

//     texture::beginTextureLoadProcess();
//     Mix_PlayChannel(0, firstStartSfx, 0);
// }

// void MainMenuScene::deinit() const
// {
//     Mix_FreeChunk(firstStartSfx);
//     SDL_DestroyTexture(backgroundImage.Resource);
//     SDL_DestroyTexture(logo.Resource);
//     // SDL_DestroyTexture((*buttons[0].getLabel()).Resource);
// }

// void MainMenuScene::redraw() const
// {
//     //const auto renderer = Game::Renderer;
//     SDL_Renderer* const renderer = globals::Properties.value().Renderer;
//     utility::changeDrawColorTo(renderer, col::BLACK);
//     SDL_RenderClear(renderer);

//     if (sceneLoaded)
//     {
//         utility::drawTextureRegion(renderer, backgroundImage);
//         utility::drawTextureRegion(renderer, logo);

//         fut::forEach(buttons, [](const auto& button, const size_t /*i*/) {
//             button.draw();
//         });
//     }

//     SDL_RenderPresent(renderer);
// }

// void MainMenuScene::intersects(const SDL_FPoint location)
// {
//     TextBubble* newlySelectedButton = NULL;

//     fut::forEach(buttons, [&](const auto& button, const size_t i)
//     {
//         if (button.isInsideItsHitbox(location))
//         {
//             newlySelectedButton = &buttons[i];
//         }
//     });

//     if (selectedButton != newlySelectedButton)
//     {
//         if (selectedButton != NULL)
//         {
//             (*selectedButton).disableHover();
//         }

//         selectedButton = newlySelectedButton;

//         if (selectedButton != NULL)
//         {
//             (*selectedButton).enableHover();
//         }

//         utility::invalidate();
//     }
// }

// void MainMenuScene::enable()
// {
//     sceneLoaded = true;
// }

// SDL_FRect MainMenuScene::initializeLogo()
// {
//     const int32_t screenWidth = globals::Properties.value().ScreenWidth;
//     const int32_t screenHeight = globals::Properties.value().ScreenHeight;

//     const uint32_t halfScreenHeight = screenHeight / 2;
//     const auto fittingLogoTexture = texture::findTextureThatFitsIntoArea(halfScreenHeight, halfScreenHeight, "logo");
    
//     texture::queueTextureLoad({
//         .Source = WhereToLoadTextureFrom::FromDisk,
//         .Asset = fittingLogoTexture.Path,
//         .Output = &logo,
//     });

//     return SDL_FRect{
//         .x = screenWidth / 2.0f - fittingLogoTexture.Width / 2.0f,
//         .y = (halfScreenHeight - fittingLogoTexture.Height) / 2.0f,
//         .w = static_cast<float>(fittingLogoTexture.Width),
//         .h = static_cast<float>(fittingLogoTexture.Height),
//     };
// }

// SDL_FRect MainMenuScene::retrievePositionOfButton(const uint32_t index) const
// {
//     const int32_t screenHeight = globals::Properties.value().ScreenHeight;

//     const uint32_t halfScreenHeight = screenHeight / 2;
//     const float totalItemSpace = halfScreenHeight * 0.7f;
//     const float individualItemSpace = totalItemSpace / BUTTON_COUNT;
//     const float totalPaddingSpace = halfScreenHeight - totalItemSpace;
//     const float individualPaddingSpace = totalPaddingSpace / (BUTTON_COUNT + 1);

//     return SDL_FRect{
//         .x = (*logo.Area).x,
//         .y = halfScreenHeight + index * individualPaddingSpace + (index - 1) * individualItemSpace,
//         .w = (*logo.Area).w,
//         .h = individualItemSpace,
//     };
// }

// void MainMenuScene::clicks()
// {
//     if (selectedButton != NULL)
//     {
//         (*selectedButton).click();
//     }
// }

#pragma once
#include <cstdint>
#include "SDL3_mixer/SDL_mixer.h"
#include "color.hpp"
#include "debug.hpp"
#include "fontmanager.hpp"
#include "texture.hpp"
#include "functionals.hpp"
#include "utility.hpp"
#include "globals.hpp"
#include "textbubble.hpp"

using enum Utility::CustomEvents;

class MainMenuScene
{
private:
    static constexpr uint32_t BUTTON_COUNT = 4;
    bool sceneLoaded;
    TextureRegion backgroundImage;
    TextureRegion logo;
    std::array<TextBubble, BUTTON_COUNT> buttons;
    TextBubble* selectedButton;
    Mix_Chunk* firstStartSfx;

private:
    SDL_FRect initializeLogo()
    {
        const int32_t screenWidth = Globals::Properties.value().ScreenWidth;
        const int32_t screenHeight = Globals::Properties.value().ScreenHeight;

        const uint32_t halfScreenHeight = screenHeight / 2;
        const auto fittingLogoTexture = texture::findTextureThatFitsIntoArea(halfScreenHeight, halfScreenHeight, "logo");
        
        texture::queueTextureLoad({
            .Source = WhereToLoadTextureFrom::FromDisk,
            .Asset = fittingLogoTexture.Path,
            .Output = &logo,
        });

        return SDL_FRect{
            .x = screenWidth / 2.0f - fittingLogoTexture.Width / 2.0f,
            .y = (halfScreenHeight - fittingLogoTexture.Height) / 2.0f,
            .w = static_cast<float>(fittingLogoTexture.Width),
            .h = static_cast<float>(fittingLogoTexture.Height),
        };
    }

    SDL_FRect retrievePositionOfButton(const uint32_t index) const
    {
        const int32_t screenHeight = Globals::Properties.value().ScreenHeight;

        const uint32_t halfScreenHeight = screenHeight / 2;
        const float totalItemSpace = halfScreenHeight * 0.7f;
        const float individualItemSpace = totalItemSpace / BUTTON_COUNT;
        const float totalPaddingSpace = halfScreenHeight - totalItemSpace;
        const float individualPaddingSpace = totalPaddingSpace / (BUTTON_COUNT + 1);

        return SDL_FRect({
            .x = logo.Area.value().x,
            .y = halfScreenHeight + index * individualPaddingSpace + (index - 1) * individualItemSpace,
            .w = logo.Area.value().w,
            .h = individualItemSpace,
        });
    }

public:
    MainMenuScene()
        : sceneLoaded(false)
        , logo(initializeLogo())
        , buttons({
              { retrievePositionOfButton(1), "New Game", []() { Utility::requestEvent({ .user = { .type = EVENT_CHANGE_SCENE, .code = 2, } }); } },
              { retrievePositionOfButton(2), "How to Play", []() { Utility::requestEvent({ .type = SDL_EVENT_QUIT }); } },
              { retrievePositionOfButton(3), "Settings", []() { Utility::requestEvent({ .type = SDL_EVENT_QUIT }); } },
              { retrievePositionOfButton(4), "Quit", []() { Utility::requestEvent({ .type = SDL_EVENT_QUIT }); } },
          })
        , selectedButton(NULL)
        , firstStartSfx(Mix_LoadWAV("assets/audio/main-theme.mp3"))
    {
        uint32_t foundFontSize = FontManager::findSuitableFontSize(buttons);
        ASSERT(foundFontSize > 0, "Could not find valid font size for scene");

        MultiSizeTexture background = texture::findTextureThatFitsIntoArea(
            Globals::Properties.value().ScreenWidth, //Game::ScreenWidth,
            Globals::Properties.value().ScreenHeight, //Game::ScreenHeight,
            "background"
        );
        
        texture::queueTextureLoad({
            .Source = WhereToLoadTextureFrom::FromDisk,
            .Asset = background.Path,
            .Output = &backgroundImage,
        });

        forEach(buttons, [&](auto&& button, const size_t i)
        {
            texture::queueTextureLoad({
                .Source = WhereToLoadTextureFrom::FromText,
                .Asset = button.getText(),
                .Output = buttons[i].getLabel(),
                .HoldingArea = button.getHoldingArea(),
            });
        });

        texture::beginTextureLoadProcess();
        Mix_PlayChannel(0, firstStartSfx, 0);
        Utility::invalidate();
    }


    void deinit() const
    {
        Mix_FreeChunk(firstStartSfx);
        SDL_DestroyTexture(backgroundImage.Resource);
        SDL_DestroyTexture(logo.Resource);
        // SDL_DestroyTexture((*buttons[0].getLabel()).Resource);
    }

    void redraw() const
    {
        SDL_Renderer* const renderer = Globals::Properties.value().Renderer;
        Utility::changeDrawColorTo(renderer, col::BLACK);
        SDL_RenderClear(renderer);

        if (sceneLoaded)
        {
            Utility::drawTextureRegion(renderer, backgroundImage);
            Utility::drawTextureRegion(renderer, logo);

            forEach(buttons, [](const auto& button, const size_t /*i*/) {
                button.draw();
            });
        }

        SDL_RenderPresent(renderer);
    }

    void intersects(const SDL_FPoint location)
    {
        TextBubble* newlySelectedButton = NULL;

        forEach(buttons, [&](const auto& button, const size_t i)
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

            Utility::invalidate();
        }
    }

    void enable()
    {
        sceneLoaded = true;
    }

    void clicks() const
    {
        if (selectedButton != NULL)
        {
            (*selectedButton).click();
        }
    }
};
