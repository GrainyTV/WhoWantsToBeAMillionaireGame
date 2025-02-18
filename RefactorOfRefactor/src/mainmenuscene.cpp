#include "mainmenuscene.hpp"
#include "SDL3/SDL.h"
#include "asset.hpp"
#include "fontmanager.hpp"
#include "functionals.hpp"
#include "globals.hpp"
#include "hexagon.hpp"
#include "invokable.hpp"
#include "textbubble.hpp"
#include "textureregion.hpp"
#include "utility.hpp"

using enum Utility::CustomEvents;
static constexpr size_t BUTTON_COUNT = 4;

static SDL_FRect initializeLogoArea()
{
    const auto properties = Globals::Properties.value();
    const int32_t screenWidth = properties.ScreenWidth;
    const int32_t screenHeight = properties.ScreenHeight;

    const uint32_t halfScreenHeight = screenHeight / 2;
    const float logoSize = halfScreenHeight * 0.9f;

    return {
        .x = screenWidth / 2.0f - logoSize / 2.0f,
        .y = (halfScreenHeight - logoSize) / 2.0f,
        .w = logoSize,
        .h = logoSize,
    };
}

static std::array<TextBubble, BUTTON_COUNT> initializeButtons(const SDL_FRect logoArea)
{
    const auto properties = Globals::Properties.value();
    const int32_t screenHeight = properties.ScreenHeight;

    const uint32_t halfScreenHeight = screenHeight / 2;
    const float totalItemSpace = halfScreenHeight * 0.7f;
    const float individualItemSpace = totalItemSpace / BUTTON_COUNT;
    const float totalPaddingSpace = halfScreenHeight - totalItemSpace;
    const float individualPaddingSpace = totalPaddingSpace / (BUTTON_COUNT + 1);

    const std::vector<SDL_FRect> buttonAreas = 
        Seq::range<1, 5>()
        | Seq::map([&](const int32_t index)
            {
                return SDL_FRect({
                    .x = logoArea.x,
                    .y = halfScreenHeight + index * individualPaddingSpace + (index - 1) * individualItemSpace,
                    .w = logoArea.w,
                    .h = individualItemSpace,
                });
            });

    using enum Utility::CustomEvents;

    return { 
        TextBubble({ 
            .Frontend = Hexagon(buttonAreas[0], "New Game"),
            .Backend = Invokable([]() { Utility::requestUserEvent({ .type = EVENT_CHANGE_SCENE_INGAME }); })
        }),
        
        TextBubble({ 
            .Frontend = Hexagon(buttonAreas[1], "How to Play"),
            .Backend = Invokable()
        }),
        
        TextBubble({ 
            .Frontend = Hexagon(buttonAreas[2], "Settings"),
            .Backend = Invokable()
        }),
        
        TextBubble({ 
            .Frontend = Hexagon(buttonAreas[3], "Quit"),
            .Backend = Invokable()
        })
    };
}

MainMenuScene::MainMenuScene()
    : sceneLoaded(false)
    , selectedButton(Option::None<int32_t>())
    , logoArea(initializeLogoArea())
    , uiButtons(initializeButtons(logoArea))
{
    ASSETS | Seq::iter([](const Asset::Identifier iden) { Asset::queueToLoad(iden); });
    Asset::beginLoadProcess();

    Utility::invalidate();
}

void MainMenuScene::deinit() const
{
    //Mix_FreeChunk(firstStartSfx);
    //SDL_DestroyTexture(backgroundImage.Resource);
    //SDL_DestroyTexture(logo.Resource);
    // SDL_DestroyTexture((*buttons[0].getLabel()).Resource);
}

void MainMenuScene::redraw() const
{
    SDL_Renderer* const renderer = Globals::Properties.value().Renderer;
    Utility::changeDrawColorTo(renderer, Color::BLACK);
    SDL_RenderClear(renderer);

    if (sceneLoaded)
    {
        Utility::drawTextureRegion(renderer, TextureRegion(Asset::getTextureById(Background)));
        Utility::drawTextureRegion(renderer, TextureRegion(Asset::getTextureById(Logo), logoArea));

        uiButtons[0].Frontend.draw();
        uiButtons[1].Frontend.draw();
        uiButtons[2].Frontend.draw();
        uiButtons[3].Frontend.draw();
    }

    SDL_RenderPresent(renderer);
}

void MainMenuScene::intersects(const SDL_FPoint /*location*/)
{
    // auto newlySelectedButton = Option::None<int32_t>();

    // Seq::range<0, BUTTON_COUNT>()
    // | Seq::iter([&](int32_t i)
    //     {
    //         if (uiButtons[i].isInsideItsHitbox(location))
    //         {
    //             newlySelectedButton = Option::Some(i);
    //         }
    //     });

    // if (selectedButton.isSome())
    // {
    //     const int32_t previousIndex = selectedButton.value();

    //     if (newlySelectedButton.isNone() || (newlySelectedButton.isSome() && newlySelectedButton.value() != previousIndex))
    //     {
    //         uiButtons[previousIndex].disableHover();
    //         selectedButton = Option::None<int32_t>();
    //         Utility::invalidate();
    //     }
    // }

    // if (newlySelectedButton.isSome())
    // {
    //     const int32_t newIndex = newlySelectedButton.value();
        
    //     if (selectedButton.isNone() || selectedButton.value() != newIndex)
    //     {
    //         uiButtons[newIndex].enableHover();
    //         selectedButton = newlySelectedButton;
    //         Utility::invalidate();
    //     }
    // }
}

void MainMenuScene::enable()
{
    sceneLoaded = true;
}

void MainMenuScene::clicks()
{
    // if (selectedButton.isSome())
    // {
    //     const int32_t index = selectedButton.value();
    //     buttons[index].click();
    // }

    uiButtons[0].Backend.execute();
}
