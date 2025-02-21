#include "ingamescene.hpp"

std::array<TextBubble, InGameScene::BUTTON_COUNT> InGameScene::initializeButtons()
{
    const int32_t screenWidth = Globals::Properties.value().ScreenWidth;
    const int32_t screenHeight = Globals::Properties.value().ScreenHeight;

    const float sixtyPercantOfHeight = 0.6f * screenHeight;
    const float totalAvailableSpace = screenHeight - sixtyPercantOfHeight;
    const float individualItemSpace = totalAvailableSpace * 0.75f / (BUTTON_COUNT - 2);
    const float individualPaddingSpace = totalAvailableSpace * 0.25f / (BUTTON_COUNT - 2 + 1);

    const float sixtyfivePercantOfWidth = 0.65f * screenWidth;
    const float beginWidth = (screenWidth - sixtyfivePercantOfWidth) / 2.0f;
    
    const std::vector<SDL_FRect> buttonAreas = 
        Seq::range<1, 6>()
        | Seq::map([&](const int32_t& index)
            {
                const size_t row = index / 2;

                return SDL_FRect({
                    .x = index == 1 ? beginWidth : index % 2 == 0 ? beginWidth : beginWidth + sixtyfivePercantOfWidth * 0.55f,
                    .y = sixtyPercantOfHeight + (row + 1) * individualPaddingSpace + row * individualItemSpace,
                    .w = index == 1 ? sixtyfivePercantOfWidth : sixtyfivePercantOfWidth * 0.45f,
                    .h = individualItemSpace,
                });
            });

    const Data chosenData = Asset::getRandomData();

    return {
        TextBubble({
            .Frontend = Hexagon(buttonAreas[0], chosenData.question),
            .Backend = Invokable()
        }),
        TextBubble({
            .Frontend = Hexagon(buttonAreas[1], chosenData.answers[0].Text),
            .Backend = Invokable()
        }),
        TextBubble({
            .Frontend = Hexagon(buttonAreas[2], chosenData.answers[1].Text, false),
            .Backend = Invokable()
        }),
        TextBubble({
            .Frontend = Hexagon(buttonAreas[3], chosenData.answers[2].Text),
            .Backend = Invokable()
        }),
        TextBubble({
            .Frontend = Hexagon(buttonAreas[4], chosenData.answers[3].Text, false),
            .Backend = Invokable()
        }),
    };
}

InGameScene::InGameScene()
    : sceneEnabled(false)
    , selectedButton(Option::None<int32_t>())
    , uiButtons(initializeButtons())
{
    ASSETS | Seq::iter([](const Asset::Identifier iden) { Asset::queueToLoad(iden); });
    Asset::beginLoadProcess();

    Utility::invalidate();
}

void InGameScene::deinit() const
{
}

void InGameScene::redraw() const
{
    SDL_Renderer* const renderer = Globals::Properties.value().Renderer;
    Utility::changeDrawColorTo(renderer, Color::BLACK);
    SDL_RenderClear(renderer);

    if (sceneEnabled)
    {
        Utility::drawTextureRegion(renderer, TextureRegion(Asset::getTextureById(Background)));

        uiButtons[0].Frontend.draw();
        uiButtons[1].Frontend.draw();
        uiButtons[2].Frontend.draw();
        uiButtons[3].Frontend.draw();
        uiButtons[4].Frontend.draw();
    }

    SDL_RenderPresent(renderer);
}

void InGameScene::intersects(const SDL_FPoint /*location*/)
{
}

void InGameScene::enable()
{
    sceneEnabled = true;

    Mix_PlayMusic(Asset::getMusicById(MusicEasy), -1);
}

void InGameScene::clicks()
{
}
