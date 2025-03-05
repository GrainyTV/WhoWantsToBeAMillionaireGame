#include "ingamescene.hpp"
#include "asset.hpp"
#include "functionals.hpp"
#include "globals.hpp"
#include "hexagon.hpp"
#include "optionwrap.hpp"
#include "utility.hpp"

namespace
{
    constexpr size_t BUTTON_COUNT = 5;
    bool sceneLoaded = false;
    Option::Inst<size_t> selectedButton = Option::None<size_t>();
    std::array<TextBubble, BUTTON_COUNT> uiButtons;

    using enum Asset::Identifier;

    std::array<TextBubble, BUTTON_COUNT> initializeButtons()
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

        return {
            TextBubble({
                .Frontend = Hexagon::init(buttonAreas[0], true),
                .Backend = Invokable()
            }),
            TextBubble({
                .Frontend = Hexagon::init(buttonAreas[1], true),
                .Backend = Invokable()
            }),
            TextBubble({
                .Frontend = Hexagon::init(buttonAreas[2], false),
                .Backend = Invokable()
            }),
            TextBubble({
                .Frontend = Hexagon::init(buttonAreas[3], true),
                .Backend = Invokable()
            }),
            TextBubble({
                .Frontend = Hexagon::init(buttonAreas[4], false),
                .Backend = Invokable()
            }),
        };
    }
}

void InGameScene::init()
{
    uiButtons = initializeButtons();

    Asset::queueToLoad(Background);
    Asset::queueToLoad(MusicEasy);
    Asset::queueToLoad(MusicMedium);
    Asset::queueToLoad(MusicHard);
    Asset::queueToLoad(InputFiles);
    Asset::beginLoadProcess();

    Utility::invalidate();
}

void InGameScene::deinit()
{
}

void InGameScene::redraw()
{
    OpenGL::clearScreen();

    if (sceneLoaded)
    {
        OpenGL::renderTexture(Asset::getTextureById(Background));

        Hexagon::draw(uiButtons[0].Frontend.GpuProperties, false);
        Hexagon::draw(uiButtons[1].Frontend.GpuProperties, false);
        Hexagon::draw(uiButtons[2].Frontend.GpuProperties, false);
        Hexagon::draw(uiButtons[3].Frontend.GpuProperties, false);
        Hexagon::draw(uiButtons[4].Frontend.GpuProperties, false);
    }

    const auto properties = Globals::Properties.value();
    SDL_GL_SwapWindow(properties.Window);
}

void InGameScene::intersects(const SDL_FPoint /*location*/)
{
}

void InGameScene::onSceneLoaded()
{
    sceneLoaded = true;
    Mix_PlayMusic(Asset::getMusicById(MusicEasy), -1);
    
    const auto chosenData = Asset::getRandomData();
    Hexagon::lateinit(uiButtons[0].Frontend, chosenData.Question);
    Hexagon::lateinit(uiButtons[1].Frontend, chosenData.AnswerA.Text);
    Hexagon::lateinit(uiButtons[2].Frontend, chosenData.AnswerB.Text);
    Hexagon::lateinit(uiButtons[3].Frontend, chosenData.AnswerC.Text);
    Hexagon::lateinit(uiButtons[4].Frontend, chosenData.AnswerD.Text);

    Utility::invalidate();
}

void InGameScene::clicks()
{
}
