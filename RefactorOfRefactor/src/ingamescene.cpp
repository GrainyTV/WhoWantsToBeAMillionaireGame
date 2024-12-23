#include "ingamescene.hpp"
#include "globals.hpp"
#include "utility.hpp"
#include "functionals.hpp"
#include "texture.hpp"
#include "textbubble.hpp"

using namespace Functionals;

static bool sceneLoaded = false;
static constexpr size_t BUTTON_COUNT = 5;
static std::array<TextBubble, BUTTON_COUNT> buttons;

static void initializeButtons()
{
    const int32_t screenWidth = Globals::Properties.value().ScreenWidth;
    const int32_t screenHeight = Globals::Properties.value().ScreenHeight;

    const float sixtyPercantOfHeight = 0.6f * screenHeight;
    const float totalAvailableSpace = screenHeight - sixtyPercantOfHeight;
    const float individualItemSpace = totalAvailableSpace * 0.75f / (BUTTON_COUNT - 2);
    const float individualPaddingSpace = totalAvailableSpace * 0.25f / (BUTTON_COUNT - 2 + 1);

    const float sixtyfivePercantOfWidth = 0.65f * screenWidth;
    const float beginWidth = (screenWidth - sixtyfivePercantOfWidth) / 2.0f;
    
    const std::vector<SDL_FRect> buttonAreas = map(range<1, 6>(), [&](const int32_t& index)
    {
        const size_t row = index / 2;

        return SDL_FRect({
            .x = index == 1 ? beginWidth : index % 2 == 0 ? beginWidth : beginWidth + sixtyfivePercantOfWidth * 0.55f,
            .y = sixtyPercantOfHeight + (row + 1) * individualPaddingSpace + row * individualItemSpace,
            .w = index == 1 ? sixtyfivePercantOfWidth : sixtyfivePercantOfWidth * 0.45f,
            .h = individualItemSpace,
        });
    });

    buttons[0].Area = buttonAreas[0];
    buttons[1].Area = buttonAreas[1];
    buttons[2].Area = buttonAreas[2];
    buttons[3].Area = buttonAreas[3];
    buttons[4].Area = buttonAreas[4];

    buttons[0].init("Question", []() {});
    buttons[1].init("Answer A", []() {});
    buttons[2].init("Answer B", []() {}, false);
    buttons[3].init("Answer C", []() {});
    buttons[4].init("Answer D", []() {}, false);
}

InGameScene::InGameScene()
{
    initializeButtons();
    
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
}

void InGameScene::deinit() const
{
}

void InGameScene::redraw() const
{
    SDL_Renderer* const renderer = Globals::Properties.value().Renderer;
    Utility::changeDrawColorTo(renderer, col::BLACK);
    SDL_RenderClear(renderer);

    if (sceneLoaded)
    {
        Utility::drawTextureRegion(renderer, Globals::BackgroundImage);

        forEach(buttons, [](const TextBubble& button)
        {
            button.draw();
        });
    }

    SDL_RenderPresent(renderer);
}

void InGameScene::intersects(const SDL_FPoint /*location*/)
{
}

void InGameScene::enable()
{
    sceneLoaded = true;
}

void InGameScene::clicks()
{
}