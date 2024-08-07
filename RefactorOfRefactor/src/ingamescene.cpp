#include "ingamescene.hpp"
#include "colors.hpp"
#include "game.hpp"

InGameScene::InGameScene()
    : sceneLoaded(false)
    , buttons({
          { retrievePositionOfButton(1), "Answer A", []() {} },
          { retrievePositionOfButton(2), "Answer B", []() {} },
          { retrievePositionOfButton(3), "Answer C", []() {} },
          { retrievePositionOfButton(4), "Answer D", []() {} },
      })
{
    fut::forEach(buttons, [&](auto&& button, const size_t i) {
        Game::TextureLoader.queueTextureLoad({
            .Source = WhereToLoadTextureFrom::FromText,
            .Asset = button.getText(),
            .Output = buttons[i].getLabel(),
            .HoldingArea = button.getHoldingArea(),
        });
    });

    Game::TextureLoader.beginTextureLoadProcess();
}

SDL_FRect InGameScene::retrievePositionOfButton(const uint32_t index)
{
    const float sixtyPercantOfHeight = 0.6f * Game::ScreenHeight;
    const float totalAvailableSpace = Game::ScreenHeight - sixtyPercantOfHeight;
    const float individualItemSpace = totalAvailableSpace * 0.75f / BUTTON_COUNT;
    const float individualPaddingSpace = totalAvailableSpace * 0.25f / (BUTTON_COUNT + 1);

    const float sixtyfivePercantOfWidth = 0.65f * Game::ScreenWidth;
    const float beginWidth = (Game::ScreenWidth - sixtyfivePercantOfWidth) / 2.0f;

    const auto a = SDL_FRect{
        .x = index % 2 == 0 ? beginWidth : beginWidth + sixtyfivePercantOfWidth * 0.5f,
        .y = sixtyPercantOfHeight + index * individualPaddingSpace + (index - 1) * individualItemSpace,
        .w = sixtyfivePercantOfWidth * 0.5f,
        .h = individualItemSpace,
    };

    return a;
}

void InGameScene::deinit() const
{
}

void InGameScene::redraw() const
{
    const auto renderer = Game::Renderer;
    ext::changeDrawColorTo(renderer, col::BLACK);
    SDL_RenderClear(renderer);

    if (sceneLoaded) {
        
    }

    // if (sceneLoaded)
    // {
    //     fut::forEach(buttons, [](const auto& button, const size_t /*i*/) {
    //         button.draw();
    //     });
    // }

    fut::forEach(buttons, [](const auto& button, const size_t /*i*/) {
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