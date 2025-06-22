#pragma once
#include "asset.hpp"
#include "fontmanager.hpp"
#include "seq/seq.hpp"
#include "globals.hpp"
#include "hexagon.hpp"
#include "iscene.hpp"
#include "optionwrap.hpp"
#include "scoreboard.hpp"
#include "utility.hpp"

#include <future>

namespace Scene
{
    using Hexagon::TextBubble;
    using ScoreboardModule::Scoreboard;
    using enum Asset::Identifier;

    class InGameScene
    {
    private:
        bool sceneLoaded = false;
        float totalAvailableSpaceTopHalf = 0;
        float totalAvailableSpaceBottomHalf = 0;
        Option::Inst<size_t> selectedButton = Option::None<size_t>();
        
        static constexpr size_t BUTTON_COUNT = 5;
        std::array<TextBubble, BUTTON_COUNT> uiButtons;
        
        Toml::Data data;
        Scoreboard scoreboard;
        std::future<void> animationProcess;

        void removeHalfOfTheAnswers()
        {
            uiButtons.at(1).IsEnabled = false;
            uiButtons.at(2).IsEnabled = false;
            Utility::invalidate();
        }

        void buttonIntersectionWithPreviousHit(SDL_FPoint location)
        {
            const std::size_t buttonId = selectedButton.value();

            if (buttonId >= 1 && buttonId <= 4)
            {
                const bool noMainButtonsHovered =
                    Seq::range<1, BUTTON_COUNT>()
                    | Seq::forall([&](std::size_t i)
                        {
                            return Hexagon::isCursorInside(uiButtons.at(i).Frontend.CpuProperties.Positions, Utility::fPointToGlm(location)) == false;
                        });            

                if (noMainButtonsHovered)
                {
                    uiButtons.at(selectedButton.value()).Frontend.RenderProperties.ButtonColor = Color::NBLACK;  
                    selectedButton = Option::None<std::size_t>();
                    Utility::invalidate();
                }

                return;
            }

            if (buttonId == 5 && Hexagon::isCursorInside(scoreboard.FiftyFiftyHelp.CpuProperties.Vertices, Utility::fPointToGlm(location)) == false)
            {
                scoreboard.FiftyFiftyHelp.GpuProperties.Color = Color::NGRAY;
                selectedButton = Option::None<std::size_t>();
                Utility::invalidate();
                return;
            }

            if (buttonId == 6 && Hexagon::isCursorInside(scoreboard.PhoneHelp.CpuProperties.Vertices, Utility::fPointToGlm(location)) == false)
            {
                scoreboard.PhoneHelp.GpuProperties.Color = Color::NGRAY;
                selectedButton = Option::None<std::size_t>();
                Utility::invalidate();
                return;     
            }

            if (buttonId == 7 && Hexagon::isCursorInside(scoreboard.AudienceHelp.CpuProperties.Vertices, Utility::fPointToGlm(location)) == false)
            {
                scoreboard.AudienceHelp.GpuProperties.Color = Color::NGRAY;
                selectedButton = Option::None<std::size_t>();
                Utility::invalidate();
            }
        }

        Option::Inst<std::size_t> buttonIntersectionWithoutPreviousHit(SDL_FPoint location)
        {
            auto selectedButton = Option::None<std::size_t>();

            Seq::range<1, BUTTON_COUNT>()
            | Seq::iter([&](std::size_t i)
                {
                    if (Hexagon::isCursorInside(uiButtons.at(i).Frontend.CpuProperties.Positions, Utility::fPointToGlm(location))
                        && uiButtons.at(i).IsEnabled)
                    {
                        selectedButton = Option::Some(i);
                    }
                });

            if (selectedButton.isSome())
            {
                return selectedButton;
            }

            if (Hexagon::isCursorInside(scoreboard.FiftyFiftyHelp.CpuProperties.Vertices, Utility::fPointToGlm(location)))
            {
                return Option::Some(BUTTON_COUNT);
            }

            if (Hexagon::isCursorInside(scoreboard.PhoneHelp.CpuProperties.Vertices, Utility::fPointToGlm(location)))
            {
                return Option::Some(BUTTON_COUNT + 1);
            }

            if (Hexagon::isCursorInside(scoreboard.AudienceHelp.CpuProperties.Vertices, Utility::fPointToGlm(location)))
            {
                return Option::Some(BUTTON_COUNT + 2);
            }

            return Option::None<std::size_t>();
        }

        bool checkForOngoingAnimation()
        {
            if (animationProcess.valid())
            {
                const std::future_status animStatus = animationProcess.wait_for(std::chrono::milliseconds(0));
                
                if (animStatus != std::future_status::ready)
                {
                    return true;
                }

                Utility::resetAsync(animationProcess);
            }

            return false;
        }

        void animateTextPopups()
        {
            Seq::range<0, BUTTON_COUNT>()
            | Seq::iter([this](std::size_t i)
                {
                    std::this_thread::sleep_for(std::chrono::milliseconds(i == 0 ? 500 : 2000));
                    uiButtons.at(i).Frontend.RenderProperties.TextVisible = true;
                    Utility::invalidate();
                });

            std::this_thread::sleep_for(std::chrono::milliseconds(25));
        }

        void ontoTheNextRound()
        {
            Seq::range<0, BUTTON_COUNT>()
            | Seq::iter([this](std::size_t i)
                {
                    uiButtons.at(i).Frontend.RenderProperties.TextVisible = false;
                    uiButtons.at(i).Frontend.RenderProperties.ButtonColor = Color::NBLACK;
                });
            
            data = Asset::getRandomData();

            Hexagon::lateinit(uiButtons[0].Frontend, data.Question);
            Hexagon::lateinit(uiButtons[1].Frontend, data.AnswerA.Text);
            Hexagon::lateinit(uiButtons[2].Frontend, data.AnswerB.Text);
            Hexagon::lateinit(uiButtons[3].Frontend, data.AnswerC.Text);
            Hexagon::lateinit(uiButtons[4].Frontend, data.AnswerD.Text);

            animationProcess = std::async(std::launch::async, &InGameScene::animateTextPopups, this);
        }

        void animateAnswerValidation()
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(2250));
            Mix_PlayChannel(-1, Asset::getSfxById(CorrectAnswer), 0);
            Utility::invalidate();

            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            auto* const callback = new Invokable(&InGameScene::ontoTheNextRound, this);
            Utility::requestUserEvent({ .type = Utility::EVENT_INVOKE_ON_UI_THREAD, .data1 = callback });
        }

        void checkIfAnswerIsCorrect(Toml::Answer* answer)
        {
            Mix_PlayChannel(-1, Asset::getSfxById(FinalAnswer), 0);

            if ((*answer).IsCorrect)
            {
                uiButtons.at(1).Frontend.RenderProperties.ButtonColor = Color::NGREEN;
                animationProcess = std::async(std::launch::async, &InGameScene::animateAnswerValidation, this);          
            }
            else
            {
                uiButtons.at(2).Frontend.RenderProperties.ButtonColor = Color::NRED;
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
            Asset::queueToLoad(HELP_AUDIENCE);
            Asset::queueToLoad(HELP_PHONE);
            Asset::beginLoadProcess();

            Utility::invalidate();
        }

        void deinit()
        {
        }

        void redraw()
        {
            OpenGL::clearScreen();
            OpenGL::renderTexture(Asset::getTextureById(Background));

            if (sceneLoaded)
            {
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
            if (checkForOngoingAnimation() == false)
            {
                if (selectedButton.isSome())
                {
                    buttonIntersectionWithPreviousHit(location);
                }
                else
                {
                    const Option::Inst<std::size_t> hit = buttonIntersectionWithoutPreviousHit(location);

                    if (hit.isSome())
                    {
                        const std::size_t buttonId = hit.value();

                        switch (buttonId)
                        {
                            case 1:
                            case 2:
                            case 3:
                            case 4:
                                uiButtons.at(buttonId).Frontend.RenderProperties.ButtonColor = Color::NORANGE;
                                break;

                            case 5:
                                scoreboard.FiftyFiftyHelp.GpuProperties.Color = Color::NORANGE;
                                break;
                            
                            case 6:
                                scoreboard.PhoneHelp.GpuProperties.Color = Color::NORANGE;
                                break;

                            case 7:
                                scoreboard.AudienceHelp.GpuProperties.Color = Color::NORANGE;
                                break;

                            default:
                                Debug::assert(false, "Undefined button id encountered ({})", buttonId);
                        }

                        selectedButton = hit;
                        Utility::invalidate();
                    }
                }
            }
        }

        void onSceneLoaded()
        {
            sceneLoaded = true;
            ScoreboardModule::lateinit(scoreboard.FiftyFiftyHelp, "50:50");
            ScoreboardModule::lateinit(scoreboard.PhoneHelp, HELP_PHONE);
            ScoreboardModule::lateinit(scoreboard.AudienceHelp, HELP_AUDIENCE);

            Mix_PlayMusic(Asset::getMusicById(MusicEasy), -1);
            ontoTheNextRound();

            Utility::invalidate();
        }

        void clicks()
        {
            if (checkForOngoingAnimation() == false && selectedButton.isSome())
            {
                const std::size_t buttonId = selectedButton.value();

                switch (buttonId)
                {
                    case 1:
                    case 2:
                    case 3:
                    case 4:
                        if (uiButtons.at(buttonId).IsEnabled) { uiButtons.at(buttonId).Backend.execute(); }
                        break;

                    case 5:
                        removeHalfOfTheAnswers();
                        //scoreboard.FiftyFiftyHelp.GpuProperties.Color = Color::NORANGE;
                        break;
                    
                    case 6:
                        //scoreboard.PhoneHelp.GpuProperties.Color = Color::NORANGE;
                        break;

                    case 7:
                        //scoreboard.AudienceHelp.GpuProperties.Color = Color::NORANGE;
                        break;

                    default:
                        Debug::assert(false, "Undefined button id encountered ({})", buttonId);
                }
            }
        }
    };
}


