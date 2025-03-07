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
    bool mouseEnabled = false;
    Option::Inst<size_t> selectedButton = Option::None<size_t>();
    std::array<TextBubble, BUTTON_COUNT> uiButtons;
    std::array<HexagonRenderProperties, BUTTON_COUNT> uiButtonProperties;
    Toml::Data data;

    using enum Asset::Identifier;

    void ontoTheNextRound()
    {
        auto animateTextPopups = []()
        {
            auto enableDisplayAnimation = [](void* userData, uint32_t, uint32_t) -> uint32_t
            {
                auto* enabled = static_cast<bool*>(userData);
                *enabled = true;
                Utility::invalidate();
                return 0;
            };

            uiButtonProperties
            | Seq::iteri([&](HexagonRenderProperties& props, const size_t i)
                {
                    SDL_AddTimer(500 + i * 2000, enableDisplayAnimation, &props.TextVisible);
                });

            SDL_AddTimer(8525, enableDisplayAnimation, &mouseEnabled);
        };

        uiButtonProperties | Seq::iter([](HexagonRenderProperties& props)
            {
                props.ButtonColor = Color::NBLACK;
                props.TextVisible = false;
            });
        
        data = Asset::getRandomData();

        Hexagon::lateinit(uiButtons[0].Frontend, data.Question);
        Hexagon::lateinit(uiButtons[1].Frontend, data.AnswerA.Text);
        Hexagon::lateinit(uiButtons[2].Frontend, data.AnswerB.Text);
        Hexagon::lateinit(uiButtons[3].Frontend, data.AnswerC.Text);
        Hexagon::lateinit(uiButtons[4].Frontend, data.AnswerD.Text);

        animateTextPopups();
        Utility::invalidate();
    }

    void checkIfAnswerIsCorrect(Toml::Answer* answer)
    {
        auto waitBeforeNewRoundAnimation = [](void*, uint32_t, uint32_t) -> uint32_t
        {
            auto* callback = new Invokable(ontoTheNextRound);

            Utility::requestUserEvent({
                .type = Utility::EVENT_INVOKE_ON_UI_THREAD,
                .data1 = callback,
            });
            
            return 0;
        };

        auto answerValidateAnimation = [](void* userData, uint32_t, uint32_t) -> uint32_t
        {
            auto* const sfx = static_cast<Mix_Chunk*>(userData);
            Mix_PlayChannel(-1, sfx, 0);
            Utility::invalidate();
            return 0;
        };

        Mix_PlayChannel(-1, Asset::getSfxById(FinalAnswer), 0);

        if ((*answer).IsCorrect)
        {
            SDL_AddTimer(2250, answerValidateAnimation, Asset::getSfxById(CorrectAnswer));
            SDL_AddTimer(2250 + 1000, waitBeforeNewRoundAnimation, nullptr);
            uiButtonProperties[1].ButtonColor = Color::NGREEN;
        }
        else
        {
            SDL_AddTimer(2250, answerValidateAnimation, Asset::getSfxById(WrongAnswer));
            uiButtonProperties[2].ButtonColor = Color::NRED;
        }
    }

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
                .Backend = Invokable(checkIfAnswerIsCorrect, &data.AnswerA)
            }),
            TextBubble({
                .Frontend = Hexagon::init(buttonAreas[2], false),
                .Backend = Invokable(checkIfAnswerIsCorrect, &data.AnswerB)
            }),
            TextBubble({
                .Frontend = Hexagon::init(buttonAreas[3], true),
                .Backend = Invokable(checkIfAnswerIsCorrect, &data.AnswerC)
            }),
            TextBubble({
                .Frontend = Hexagon::init(buttonAreas[4], false),
                .Backend = Invokable(checkIfAnswerIsCorrect, &data.AnswerD)
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
    Asset::queueToLoad(FinalAnswer);
    Asset::queueToLoad(CorrectAnswer);
    Asset::queueToLoad(WrongAnswer);
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

        uiButtons
        | Seq::iteri([&](const TextBubble& tb, const size_t i)
            {
                Hexagon::draw(tb.Frontend.GpuProperties, uiButtonProperties.at(i));
            });
    }

    const auto properties = Globals::Properties.value();
    SDL_GL_SwapWindow(properties.Window);
}

void InGameScene::intersects(const SDL_FPoint location)
{
    if (mouseEnabled)
    {
        bool anyHovered = false;

        // ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
        // ┃ Question should not be hovered so we take subarray ┃
        // ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛

        std::span<const TextBubble> hoverableButtons(uiButtons.begin() + 1, uiButtons.end());

        hoverableButtons
        | Seq::iteri([&](const TextBubble& tb, const size_t i)
            {
                const bool isInside = Hexagon::isCursorInside(tb.Frontend.CpuProperties.Positions, Utility::fPointToSvl(location));

                if (isInside)
                {
                    if (selectedButton.isNone())
                    {
                        selectedButton = Option::Some(i + 1);
                        uiButtonProperties.at(selectedButton.value()).ButtonColor = Color::NORANGE;
                        Utility::invalidate();
                    }

                    anyHovered = true;
                }
            });

        if (anyHovered == false && selectedButton.isSome())
        {
            uiButtonProperties.at(selectedButton.value()).ButtonColor = Color::NBLACK;
            selectedButton = Option::None<size_t>();
            Utility::invalidate();
        }
    }
}

void InGameScene::onSceneLoaded()
{
    sceneLoaded = true;

    Mix_PlayMusic(Asset::getMusicById(MusicEasy), -1);
    ontoTheNextRound();

    Utility::invalidate();
}

void InGameScene::clicks()
{
    if (selectedButton.isSome())
    {
        mouseEnabled = false;
        uiButtons.at(selectedButton.value()).Backend.execute();
    }
}
