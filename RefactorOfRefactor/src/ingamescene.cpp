#include "ingamescene.hpp"
#include "asset.hpp"
#include "functionals.hpp"
#include "globals.hpp"
#include "hexagon.hpp"
#include "optionwrap.hpp"
#include "utility.hpp"
#include "bezier.hpp"

namespace
{
    constexpr size_t BUTTON_COUNT = 5;
    bool sceneLoaded = false;
    bool mouseEnabled = false;
    float totalAvailableSpaceTopHalf = 0;
    float totalAvailableSpaceBottomHalf = 0;
    Option::Inst<size_t> selectedButton = Option::None<size_t>();
    std::array<TextBubble, BUTTON_COUNT> uiButtons;
    std::array<HexagonRenderProperties, BUTTON_COUNT> uiButtonProperties;
    Toml::Data data;
    Scoreboard scoreboard;

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
            | Seq::map([&](const int32_t& index)
                {
                    const size_t row = index / 2;

                    return SDL_FRect({
                        .x = index == 1 ? beginWidth : index % 2 == 0 ? beginWidth : beginWidth + sixtyfivePercantOfWidth * 0.55f,
                        //.y = sixtyPercantOfHeight + (row + 1) * individualPaddingSpace + row * individualItemSpace,
                        .y = totalAvailableSpaceTopHalf + (row + 1) * individualPaddingSpace + row * individualItemSpace,
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

    Scoreboard initializeScoreboard(std::span<const Vec2> referencePositions)
    {
        const float ninetyPercantOfAvailableArea = 0.9f * totalAvailableSpaceTopHalf;
        const float halfWidthOfButton = (referencePositions[3][0] - referencePositions[0][0]) * 0.5f;

        SDL_FRect baseArea({
            .x = referencePositions[2][0] - halfWidthOfButton,
            .y = (totalAvailableSpaceTopHalf - ninetyPercantOfAvailableArea) / 2.0f,
            .w = halfWidthOfButton,
            .h = ninetyPercantOfAvailableArea
        });

        const std::array<Vec2, 4> corners = Utility::cornersOfRectangle(baseArea);
        const float radius = std::min(baseArea.w, baseArea.h) * 0.1f;

        const Vec2& a0 = corners[0];
        Vec2 a1(a0[0] + radius, a0[1]);
        Vec2 a2(a0[0], a0[1] + radius);
        
        const Vec2& b0 = corners[1];
        Vec2 b1(b0[0] - radius, b0[1]);
        Vec2 b2(b0[0], b0[1] + radius);
        
        const Vec2& c0 = corners[2];
        Vec2 c1(c0[0] + radius, c0[1]);
        Vec2 c2(c0[0], c0[1] - radius);
        
        const Vec2& d0 = corners[3];
        Vec2 d1(d0[0] - radius, d0[1]);
        Vec2 d2(d0[0], d0[1] - radius);

        const std::array rect1 = { a1, b1, c1, d1 };
        const std::array rect2 = { a2, b2, c2, d2 };

        const std::array bezier1Controls = { a2, a0, a1 };
        std::vector<Vec2> bezier1 = Bezier::generateFromControls(bezier1Controls);
        
        const std::array bezier2Controls = { b1, b0, b2 };
        std::vector<Vec2> bezier2 = Bezier::generateFromControls(bezier2Controls);
        
        const std::array bezier3Controls = { c1, c0, c2 };
        std::vector<Vec2> bezier3 = Bezier::generateFromControls(bezier3Controls);        

        const std::array bezier4Controls = { d2, d0, d1 };
        std::vector<Vec2> bezier4 = Bezier::generateFromControls(bezier4Controls);

        std::vector<Vec2> border;
        border.insert(border.end(), bezier1.begin(), bezier1.end());
        border.insert(border.end(), bezier2.begin(), bezier2.end());
        border.insert(border.end(), bezier4.begin(), bezier4.end());
        border.insert(border.end(), bezier3.begin(), bezier3.end());

        std::vector<Vec2> borderVertices;

        border
        | Seq::pairwiseWrap()
        | Seq::iter([&](const std::pair<Vec2, Vec2>& neighbors)
            {
                const Vec2& a = neighbors.first;
                const Vec2& b = neighbors.second;

                Vec2 dir = a - b;
                Vec2 normal(dir[1], -dir[0]);
                normal.Normalise();

                std::array corners = {
                    a + 5.0f / 2.0f * normal,
                    a - 5.0f / 2.0f * normal,
                    b + 5.0f / 2.0f * normal,
                    b - 5.0f / 2.0f * normal
                };

                borderVertices.insert(borderVertices.end(), corners.begin(), corners.end());
            });

        bezier1.insert(bezier1.begin(), a1 + a2 - a0);
        bezier2.insert(bezier2.begin(), b1 + b2 - b0);
        bezier3.insert(bezier3.begin(), c1 + c2 - c0);
        bezier4.insert(bezier4.begin(), d1 + d2 - d0);

        return {
            .BaseArea = baseArea,
            .BackgroundPart1 = { OpenGL::createPrimitives(rect1) },
            .BackgroundPart2 = { OpenGL::createPrimitives(rect2) },
            .BackgroundPart3 = { .BufferId = OpenGL::createPrimitives(bezier1), .VertexCount = static_cast<uint32_t>(bezier1.size()) },
            .BackgroundPart4 = { .BufferId = OpenGL::createPrimitives(bezier2), .VertexCount = static_cast<uint32_t>(bezier2.size()) },
            .BackgroundPart5 = { .BufferId = OpenGL::createPrimitives(bezier3), .VertexCount = static_cast<uint32_t>(bezier3.size()) },
            .BackgroundPart6 = { .BufferId = OpenGL::createPrimitives(bezier4), .VertexCount = static_cast<uint32_t>(bezier4.size()) },
            .BackgroundBorder = { .BufferId = OpenGL::createPrimitives(borderVertices), .VertexCount = static_cast<uint32_t>(borderVertices.size()) }
        };
    }
}

void InGameScene::init()
{
    uiButtons = initializeButtons();
    scoreboard = initializeScoreboard(uiButtons[2].Frontend.CpuProperties.Positions);

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

void InGameScene::deinit()
{
}

void InGameScene::redraw()
{
    OpenGL::clearScreen();

    if (sceneLoaded)
    {
        OpenGL::renderTexture(Asset::getTextureById(Background));

        OpenGL::changeDrawColorTo(Color::NBLACK);
        OpenGL::renderQuad(scoreboard.BackgroundPart1.BufferId);
        OpenGL::renderQuad(scoreboard.BackgroundPart2.BufferId);
        OpenGL::renderPrimitives(scoreboard.BackgroundPart3.BufferId, 6, scoreboard.BackgroundPart3.VertexCount);
        OpenGL::renderPrimitives(scoreboard.BackgroundPart4.BufferId, 6, scoreboard.BackgroundPart4.VertexCount);
        OpenGL::renderPrimitives(scoreboard.BackgroundPart5.BufferId, 6, scoreboard.BackgroundPart5.VertexCount);
        OpenGL::renderPrimitives(scoreboard.BackgroundPart6.BufferId, 6, scoreboard.BackgroundPart6.VertexCount);
        
        OpenGL::changeDrawColorTo(Color::NBLUE);
        OpenGL::renderPrimitives(scoreboard.BackgroundBorder.BufferId, 5, scoreboard.BackgroundBorder.VertexCount);

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

void InGameScene::clicks()
{
    if (selectedButton.isSome())
    {
        mouseEnabled = false;
        uiButtons.at(selectedButton.value()).Backend.execute();
    }
}
