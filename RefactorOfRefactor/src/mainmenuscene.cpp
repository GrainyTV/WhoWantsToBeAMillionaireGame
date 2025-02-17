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

    return { 
        TextBubble({ 
            .Frontend = Hexagon(buttonAreas[0], "New Game"),
            .Backend = Invokable()
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

    // return SDL_FRect({
    //     .x = logoArea.x,
    //     .y = halfScreenHeight + index * individualPaddingSpace + (index - 1) * individualItemSpace,
    //     .w = logoArea.w,
    //     .h = individualItemSpace,
    // });

    // forEach(buttons, [&](TextBubble button, const size_t index)
    // {
    //     button.Area = SDL_FRect({
    //         .x = logoArea.x,
    //         .y = halfScreenHeight + index * individualPaddingSpace + (index - 1) * individualItemSpace,
    //         .w = logoArea.w,
    //         .h = individualItemSpace,
    //     });
    // });

    // size_t index = 1;
    // for (TextBubble& button : buttons)
    // {
    //     button.Area = SDL_FRect({
    //         .x = logoArea.x,
    //         .y = halfScreenHeight + index * individualPaddingSpace + (index - 1) * individualItemSpace,
    //         .w = logoArea.w,
    //         .h = individualItemSpace,
    //     });
    //     ++index;
    // }

    // buttons[0].Area = buttonAreas[0];
    // buttons[1].Area = buttonAreas[1];
    // buttons[2].Area = buttonAreas[2];
    // buttons[3].Area = buttonAreas[3];

    // buttons[0].init("New Game", []() { /*Utility::changeSceneTo<InGameScene>();*/ Utility::requestUserEvent({ .type = EVENT_CHANGE_SCENE }); });
    // buttons[1].init("How to Play", []() { Utility::requestEvent({ .type = SDL_EVENT_QUIT }); });
    // buttons[2].init("Settings", []() { Utility::requestEvent({ .type = SDL_EVENT_QUIT }); });
    // buttons[3].init("Quit", []() { Utility::requestEvent({ .type = SDL_EVENT_QUIT }); });
}

MainMenuScene::MainMenuScene()
    : sceneLoaded(false)
    //, logo(Option<SDL_FRect>::Some(initializeLogoArea()))
    //, buttons()
    , selectedButton(Option::None<int32_t>())
    , logoArea(initializeLogoArea())
    , uiButtons(initializeButtons(logoArea))
    //, firstStartSfx(Mix_LoadWAV("assets/audio/main-theme.mp3"))
{
    assets | Seq::iter([](const auto& asset) { Asset::queueToLoad(asset); });
    Asset::beginLoadProcess();

    //initializeLogo(logo);
    //initializeButtons(logo, buttons);

    //uint32_t foundFontSize = FontManager::findSuitableFontSize(buttons);
    //ASSERT(foundFontSize > 0, "Could not find valid font size for mainmenu");
    
    //GameProperties properties = Globals::Properties.value();

    // if (properties.FirstInit)
    // {
    //     Texture::queueToLoad({
    //         .Source = WhereToLoadTextureFrom::FromDisk,
    //         .Asset = "assets/textures/background.png",
    //         .Output = &Globals::BackgroundImage,
    //         .Filter = SDL_SCALEMODE_LINEAR,
    //     });

    //     properties.FirstInit = false;
    // }

    // forEach(buttons, [](TextBubble& button)
    // {
    //     texture::queueTextureLoad({
    //         .Source = WhereToLoadTextureFrom::FromText,
    //         .Asset = button.Text,
    //         .Output = &button.Label,
    //         .Filter = SDL_SCALEMODE_LINEAR,
    //         .HoldingArea = button.Area,
    //     });
    // });

    // buttons
    // | Seq::iter([](TextBubble& button) {
    //     Texture::queueToLoad({
    //         .Source = WhereToLoadTextureFrom::FromText,
    //         .Asset = button.Text,
    //         .Output = &button.Label,
    //         .Filter = SDL_SCALEMODE_LINEAR,
    //         .HoldingArea = button.Area,
    //     });
    //   });

    //Texture::beginLoadProcess();
    //Mix_PlayChannel(0, firstStartSfx, 0);
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
    Utility::changeDrawColorTo(renderer, col::BLACK);
    SDL_RenderClear(renderer);

    if (sceneLoaded)
    {
        //Utility::drawTextureRegion(renderer, Globals::BackgroundImage);
        //Utility::drawTextureRegion(renderer, logo);

        TextureRegion shit;
        shit.Area = Option::Some(logoArea);
        shit.Resource = Asset::getTextureById(Logo);

        TextureRegion shit2;
        shit2.Resource = Asset::getTextureById(Background);

        Utility::drawTextureRegion(renderer, shit2);
        Utility::drawTextureRegion(renderer, shit);

        uiButtons[0].Frontend.draw();
        uiButtons[1].Frontend.draw();
        uiButtons[2].Frontend.draw();
        uiButtons[3].Frontend.draw();

        // buttons
        // | Seq::iter([](const TextBubble& button)
        //     {
        //         button.draw();
        //     });
    }

    SDL_RenderPresent(renderer);
}

void MainMenuScene::intersects(const SDL_FPoint)
{
    // auto newlySelectedButton = Option::None<int32_t>();

    // uiButtons
    // | Seq::iteri([&](const auto& button, const size_t i)
    //     {
    //         if (button.isInsideItsHitbox(location))
    //         {
    //             newlySelectedButton = Option::Some((int32_t) i);
    //         }
    //     });

    // if (selectedButton.isSome())
    // {
    //     const int32_t previousIndex = selectedButton.value();

    //     if (newlySelectedButton.isNone() || (newlySelectedButton.isSome() && newlySelectedButton.value() != previousIndex))
    //     {
    //         buttons[previousIndex].disableHover();
    //         selectedButton = Option::None<int32_t>();
    //         Utility::invalidate();
    //     }
    // }

    // if (newlySelectedButton.isSome())
    // {
    //     const int32_t newIndex = newlySelectedButton.value();
        
    //     if (selectedButton.isNone() || selectedButton.value() != newIndex)
    //     {
    //         buttons[newIndex].enableHover();
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
}
