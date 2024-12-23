#include "mainmenuscene.hpp"
#include "fontmanager.hpp"
#include "ingamescene.hpp"
#include "texture.hpp"

using enum Utility::CustomEvents;
using Globals::GameProperties;

static void initializeLogo(TextureRegion& logo)
{
    const GameProperties properties = Globals::Properties.value();
    const int32_t screenWidth = properties.ScreenWidth;
    const int32_t screenHeight = properties.ScreenHeight;

    const uint32_t halfScreenHeight = screenHeight / 2;
    const float logoSize = halfScreenHeight * 0.9f;
    
    texture::queueTextureLoad({
        .Source = WhereToLoadTextureFrom::FromDisk,
        .Asset = "assets/textures/logo.png",
        .Output = &logo,
        .Filter = SDL_SCALEMODE_LINEAR,
    });

    logo.Area = Option<SDL_FRect>::Some({
        .x = screenWidth / 2.0f - logoSize / 2.0f,
        .y = (halfScreenHeight - logoSize) / 2.0f,
        .w = logoSize,
        .h = logoSize,
    });
}

static void initializeButtons(TextureRegion& logo, std::span<TextBubble> buttons)
{
    const size_t buttonCount = buttons.size();
    const GameProperties properties = Globals::Properties.value();
    const int32_t screenHeight = properties.ScreenHeight;

    const uint32_t halfScreenHeight = screenHeight / 2;
    const float totalItemSpace = halfScreenHeight * 0.7f;
    const float individualItemSpace = totalItemSpace / buttonCount;
    const float totalPaddingSpace = halfScreenHeight - totalItemSpace;
    const float individualPaddingSpace = totalPaddingSpace / (buttonCount + 1);

    const SDL_FRect logoArea = logo.Area.value();

    const std::vector<SDL_FRect> buttonAreas = map(range<1, 5>(), [&](const int32_t& index)
    {
        return SDL_FRect({
            .x = logoArea.x,
            .y = halfScreenHeight + index * individualPaddingSpace + (index - 1) * individualItemSpace,
            .w = logoArea.w,
            .h = individualItemSpace,
        });
    });

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

    buttons[0].Area = buttonAreas[0];
    buttons[1].Area = buttonAreas[1];
    buttons[2].Area = buttonAreas[2];
    buttons[3].Area = buttonAreas[3];

    buttons[0].init("New Game", []() { Utility::changeSceneTo<InGameScene>(); });
    buttons[1].init("How to Play", []() { Utility::requestEvent({ .type = SDL_EVENT_QUIT }); });
    buttons[2].init("Settings", []() { Utility::requestEvent({ .type = SDL_EVENT_QUIT }); });
    buttons[3].init("Quit", []() { Utility::requestEvent({ .type = SDL_EVENT_QUIT }); });
}

MainMenuScene::MainMenuScene()
    : sceneLoaded(false)
    // , logo(initializeLogo())
    // , buttons({
    //       { retrievePositionOfButton(1), "New Game", []() { Utility::requestEvent({ .user = { .type = EVENT_CHANGE_SCENE, .code = 2, } }); } },
    //       { retrievePositionOfButton(2), "How to Play", []() { Utility::requestEvent({ .type = SDL_EVENT_QUIT }); } },
    //       { retrievePositionOfButton(3), "Settings", []() { Utility::requestEvent({ .type = SDL_EVENT_QUIT }); } },
    //       { retrievePositionOfButton(4), "Quit", []() { Utility::requestEvent({ .type = SDL_EVENT_QUIT }); } },
    //   })
    , selectedButton(NULL)
    , firstStartSfx(Mix_LoadWAV("assets/audio/main-theme.mp3"))
{
    initializeLogo(logo);
    initializeButtons(logo, buttons);

    uint32_t foundFontSize = FontManager::findSuitableFontSize(buttons);
    ASSERT(foundFontSize > 0, "Could not find valid font size for mainmenu");
    
    GameProperties properties = Globals::Properties.value();

    if (properties.FirstInit)
    {
        texture::queueTextureLoad({
            .Source = WhereToLoadTextureFrom::FromDisk,
            .Asset = "assets/textures/background.png",
            .Output = &Globals::BackgroundImage,
            .Filter = SDL_SCALEMODE_LINEAR,
        });

        properties.FirstInit = false;
    }

    forEach(buttons, [](TextBubble& button)
    {
        texture::queueTextureLoad({
            .Source = WhereToLoadTextureFrom::FromText,
            .Asset = button.Text,
            .Output = &button.Label,
            .Filter = SDL_SCALEMODE_LINEAR,
            .HoldingArea = button.Area,
        });
    });

    texture::beginTextureLoadProcess();
    Mix_PlayChannel(0, firstStartSfx, 0);
    Utility::invalidate();
}

void MainMenuScene::deinit() const
{
    Mix_FreeChunk(firstStartSfx);
    //SDL_DestroyTexture(backgroundImage.Resource);
    SDL_DestroyTexture(logo.Resource);
    // SDL_DestroyTexture((*buttons[0].getLabel()).Resource);
}

void MainMenuScene::redraw() const
{
    SDL_Renderer* const renderer = Globals::Properties.value().Renderer;
    Utility::changeDrawColorTo(renderer, col::BLACK);
    SDL_RenderClear(renderer);

    if (sceneLoaded)
    {
        Utility::drawTextureRegion(renderer, Globals::BackgroundImage);
        Utility::drawTextureRegion(renderer, logo);

        forEach(buttons, [](const TextBubble& button) {
            button.draw();
        });
    }

    SDL_RenderPresent(renderer);
}

void MainMenuScene::intersects(const SDL_FPoint location)
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

void MainMenuScene::enable()
{
    sceneLoaded = true;
}

void MainMenuScene::clicks()
{
    if (selectedButton != NULL)
    {
        (*selectedButton).click();
    }
}
