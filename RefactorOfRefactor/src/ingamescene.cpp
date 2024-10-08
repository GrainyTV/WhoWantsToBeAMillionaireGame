#include "ingamescene.hpp"
//#include "colors.hpp"
//#include "game.hpp"
#include "globals.hpp"
#include "utility.hpp"
#include "functionals.hpp"

using namespace Functionals;

InGameScene::InGameScene()
    : sceneLoaded(false)
    , buttons({
          { retrievePositionOfButton(1), "Question", []() {} },
          { retrievePositionOfButton(2), "Answer A", []() {} },
          { retrievePositionOfButton(3), "Answer B", []() {}, false },
          { retrievePositionOfButton(4), "Answer C", []() {} },
          { retrievePositionOfButton(5), "Answer D", []() {}, false },
      })
{
    // fut::forEach(buttons, [&](auto&& button, const size_t i) {
    //     Game::TextureLoader.queueTextureLoad({
    //         .Source = WhereToLoadTextureFrom::FromText,
    //         .Asset = button.getText(),
    //         .Output = buttons[i].getLabel(),
    //         .HoldingArea = button.getHoldingArea(),
    //     });
    // });

    // Game::TextureLoader.beginTextureLoadProcess();
}

SDL_FRect InGameScene::retrievePositionOfButton(const uint32_t index)
{
    const int32_t screenWidth = Globals::Properties.value().ScreenWidth;
    const int32_t screenHeight = Globals::Properties.value().ScreenHeight;

    const float sixtyPercantOfHeight = 0.6f * screenHeight;
    const float totalAvailableSpace = screenHeight - sixtyPercantOfHeight;
    const float individualItemSpace = totalAvailableSpace * 0.75f / (BUTTON_COUNT - 2);
    const float individualPaddingSpace = totalAvailableSpace * 0.25f / (BUTTON_COUNT - 2 + 1);

    const float sixtyfivePercantOfWidth = 0.65f * screenWidth;
    const float beginWidth = (screenWidth - sixtyfivePercantOfWidth) / 2.0f;
    const uint32_t row = index / 2;

    return SDL_FRect{
        .x = index == 1 ? beginWidth : index % 2 == 0 ? beginWidth : beginWidth + sixtyfivePercantOfWidth * 0.55f,
        .y = sixtyPercantOfHeight + (row + 1) * individualPaddingSpace + row * individualItemSpace,
        .w = index == 1 ? sixtyfivePercantOfWidth : sixtyfivePercantOfWidth * 0.45f,
        .h = individualItemSpace,
    };
}

void InGameScene::deinit() const
{
}

void InGameScene::redraw() const
{
    //const auto renderer = Game::Renderer;
    SDL_Renderer* const renderer = Globals::Properties.value().Renderer;
    Utility::changeDrawColorTo(renderer, col::BLACK);
    SDL_RenderClear(renderer);

    if (sceneLoaded)
    {
    }

    // if (sceneLoaded)
    // {
    //     fut::forEach(buttons, [](const auto& button, const size_t /*i*/) {
    //         button.draw();
    //     });
    // }

    forEach(buttons, [](const auto& button, const size_t /*i*/) {
        button.draw();
    });

    SDL_RenderPresent(renderer);
}

void InGameScene::intersects(const SDL_FPoint /*location*/)
{
}

void InGameScene::enable()
{
}

void InGameScene::clicks()
{
}