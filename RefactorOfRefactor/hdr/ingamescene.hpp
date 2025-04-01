#pragma once
#include "asset.hpp"
#include "seq/seq.hpp"
#include "globals.hpp"
#include "hexagon.hpp"
#include "iscene.hpp"
#include "optionwrap.hpp"
#include "scoreboard.hpp"
#include "utility.hpp"

namespace Scene
{
    using Hexagon::TextBubble;
    using ScoreboardModule::Scoreboard;
    using enum Asset::Identifier;

    class InGameScene
    {
    private:
        static constexpr size_t BUTTON_COUNT = 5;
        bool sceneLoaded = false;
        bool mouseEnabled = false;
        float totalAvailableSpaceTopHalf = 0;
        float totalAvailableSpaceBottomHalf = 0;
        Option::Inst<size_t> selectedButton = Option::None<size_t>();
        std::array<TextBubble, BUTTON_COUNT> uiButtons;
        Toml::Data data;
        Scoreboard scoreboard;

        void ontoTheNextRound()
        {
            auto animateTextPopups = [this]()
            {
                auto enableDisplayAnimation = [](void* userData, uint32_t, uint32_t) -> uint32_t
                {
                    auto* enabled = static_cast<bool*>(userData);
                    *enabled = true;
                    Utility::invalidate();
                    return 0;
                };

                // uiButtons
                // | Seq::iteri([&](TextBubble& tb, const size_t i)
                //     {
                //         SDL_AddTimer(500 + i * 2000, enableDisplayAnimation, &tb.Frontend.RenderProperties.TextVisible);
                //     });

                SDL_AddTimer(8525, enableDisplayAnimation, &mouseEnabled);
            };

            // uiButtons
            // | Seq::iter([](TextBubble& tb)
            //     {
            //         tb.Frontend.RenderProperties.ButtonColor = Color::NBLACK;
            //         tb.Frontend.RenderProperties.TextVisible = false;
            //     });
            
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
            auto waitBeforeNewRoundAnimation = [](void* callback, uint32_t, uint32_t) -> uint32_t
            {
                //auto* callback = new Invokable(&InGameScene::ontoTheNextRound, this);

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

                auto* callback = new Invokable(&InGameScene::ontoTheNextRound, this);
                SDL_AddTimer(2250 + 1000, waitBeforeNewRoundAnimation, callback);
                
                uiButtons[1].Frontend.RenderProperties.ButtonColor = Color::NGREEN;
            }
            else
            {
                SDL_AddTimer(2250, answerValidateAnimation, Asset::getSfxById(WrongAnswer));
                uiButtons[2].Frontend.RenderProperties.ButtonColor = Color::NRED;
            }
        }

        std::array<TextBubble, BUTTON_COUNT> initializeButtons()
        {
            const auto properties = Globals::Properties.value();
            const auto screenWidth = static_cast<float>(properties.ScreenWidth);
            const auto screenHeight = static_cast<float>(properties.ScreenHeight);

            //const float sixtyPercantOfHeight = 0.6f * screenHeight;
            //const float totalAvailableSpace = screenHeight - sixtyPercantOfHeight;

            totalAvailableSpaceTopHalf = 0.6f * screenHeight;
            totalAvailableSpaceBottomHalf = screenHeight - totalAvailableSpaceTopHalf;
            
            //const float individualItemSpace = totalAvailableSpace * 0.75f / (BUTTON_COUNT - 2);
            //const float individualPaddingSpace = totalAvailableSpace * 0.25f / (BUTTON_COUNT - 2 + 1);

            const float individualItemSpace = totalAvailableSpaceBottomHalf * 0.75f / (BUTTON_COUNT - 2);
            const float individualPaddingSpace = totalAvailableSpaceBottomHalf * 0.25f / (BUTTON_COUNT - 2 + 1);

            const float sixtyfivePercantOfWidth = 0.65f * screenWidth;
            const float beginWidth = (screenWidth - sixtyfivePercantOfWidth) / 2.0f;
            
            const std::vector<SDL_FRect> buttonAreas = 
                Seq::range<1, 6>()
                | Seq::map([&](const int32_t index)
                    {
                        const int32_t row = index / 2;
                        const auto rowF = static_cast<float>(row);

                        return SDL_FRect({
                            .x = index == 1 ? beginWidth : index % 2 == 0 ? beginWidth : beginWidth + sixtyfivePercantOfWidth * 0.55f,
                            //.y = sixtyPercantOfHeight + (row + 1) * individualPaddingSpace + row * individualItemSpace,
                            .y = totalAvailableSpaceTopHalf + (rowF + 1) * individualPaddingSpace + rowF * individualItemSpace,
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
                    .Backend = Invokable(&InGameScene::checkIfAnswerIsCorrect, this, &data.AnswerA)
                }),
                TextBubble({
                    .Frontend = Hexagon::init(buttonAreas[2], false),
                    .Backend = Invokable(&InGameScene::checkIfAnswerIsCorrect, this, &data.AnswerB)
                }),
                TextBubble({
                    .Frontend = Hexagon::init(buttonAreas[3], true),
                    .Backend = Invokable(&InGameScene::checkIfAnswerIsCorrect, this, &data.AnswerC)
                }),
                TextBubble({
                    .Frontend = Hexagon::init(buttonAreas[4], false),
                    .Backend = Invokable(&InGameScene::checkIfAnswerIsCorrect, this, &data.AnswerD)
                }),
            };
        }

    public:
        void init()
        {
            uiButtons = initializeButtons();
            scoreboard = ScoreboardModule::init(uiButtons[2].Frontend.CpuProperties.AvailableArea, totalAvailableSpaceTopHalf);

            Asset::queueToLoad(Background);
            Asset::queueToLoad(MusicEasy);
            Asset::queueToLoad(MusicMedium);
            Asset::queueToLoad(MusicHard);
            Asset::queueToLoad(InputFiles);
            Asset::queueToLoad(FinalAnswer);
            Asset::queueToLoad(CorrectAnswer);
            Asset::queueToLoad(WrongAnswer);
            //Asset::queueToLoad(ScoreBoard);
            Asset::beginLoadProcess();

            Utility::invalidate();
        }

        void deinit()
        {
        }

        void redraw()
        {
            OpenGL::clearScreen();

            if (sceneLoaded)
            {
                OpenGL::renderTexture(Asset::getTextureById(Background));
                ScoreboardModule::draw(scoreboard);

                uiButtons
                | Seq::iter([&](const TextBubble& tb)
                    {
                        Hexagon::draw(tb.Frontend.GpuProperties, tb.Frontend.RenderProperties);
                    });
            }

            const auto properties = Globals::Properties.value();
            SDL_GL_SwapWindow(properties.Window);
        }

        void intersects(const SDL_FPoint location)
        {
            if (mouseEnabled)
            {
                bool anyHovered = false;

                // ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
                // ┃ Question should not be hovered so we take subarray ┃
                // ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛

                const std::span<const TextBubble> hoverableButtons(uiButtons.begin() + 1, uiButtons.end());

                hoverableButtons
                | Seq::iteri([&](const TextBubble& tb, const size_t i)
                    {
                        const bool isInside = Hexagon::isCursorInside(tb.Frontend.CpuProperties.Positions, Utility::fPointToGlm(location));

                        if (isInside)
                        {
                            if (selectedButton.isNone())
                            {
                                selectedButton = Option::Some(i + 1);
                                uiButtons.at(selectedButton.value()).Frontend.RenderProperties.ButtonColor = Color::NORANGE;
                                Utility::invalidate();
                            }

                            anyHovered = true;
                        }
                    });

                if (anyHovered == false && selectedButton.isSome())
                {
                    uiButtons.at(selectedButton.value()).Frontend.RenderProperties.ButtonColor = Color::NBLACK;
                    selectedButton = Option::None<size_t>();
                    Utility::invalidate();
                }
            }
        }

        void onSceneLoaded()
        {
            sceneLoaded = true;

            // TextureGpu scoreBoard = Asset::getTextureById(ScoreBoard);
            // SDL_FRect scoreBoardTarget({
            //     .x = uiButtons[0].Frontend.CpuProperties.Positions[2][0] - static_cast<float>(scoreBoard.Width),
            //     .y = 0,
            //     .w = static_cast<float>(scoreBoard.Width),
            //     .h = static_cast<float>(scoreBoard.Height)
            // });
            // OpenGL::defineTextureRenderLocation(scoreBoard, Option::Some(scoreBoardTarget));

            Mix_PlayMusic(Asset::getMusicById(MusicEasy), -1);
            ontoTheNextRound();

            Utility::invalidate();
        }

        void clicks()
        {
            if (selectedButton.isSome())
            {
                mouseEnabled = false;
                uiButtons.at(selectedButton.value()).Backend.execute();
            }
        }
    };
}


