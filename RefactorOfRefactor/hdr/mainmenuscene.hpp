#pragma once
#include "iscene.hpp"
#include "asset.hpp"
#include "globals.hpp"
#include "hexagon.hpp"
#include "optionwrap.hpp"
#include "seq/seq.hpp"
#include "utility.hpp"

namespace Scene
{
    using Hexagon::TextBubble;
    using enum Utility::CustomEvents;
    using enum Asset::Identifier;

    class MainMenuScene
    {
    private:
        static constexpr size_t BUTTON_COUNT = 4;
        bool sceneLoaded = false;
        Option::Inst<size_t> selectedButton = Option::None<size_t>();
        uint32_t sceneVao = 0;
        SDL_FRect logoArea;
        std::array<TextBubble, BUTTON_COUNT> uiButtons;

        static SDL_FRect initializeLogoArea()
        {
            const auto properties = Globals::Properties.value();
            const auto screenWidth = static_cast<float>(properties.ScreenWidth);
            const auto screenHeight = static_cast<float>(properties.ScreenHeight);

            const float halfScreenHeight = screenHeight / 2;
            const float logoSize = halfScreenHeight * 0.9f;

            return {
                .x = screenWidth / 2.0f - logoSize / 2.0f,
                .y = (halfScreenHeight - logoSize) / 2.0f,
                .w = logoSize,
                .h = logoSize,
            };
        }

        static std::array<TextBubble, BUTTON_COUNT> initializeButtons(const SDL_FRect logoArea)
        {
            const auto properties = Globals::Properties.value();
            const auto screenHeight = static_cast<float>(properties.ScreenHeight);

            const float halfScreenHeight = screenHeight / 2;
            const float totalItemSpace = halfScreenHeight * 0.7f;
            const float individualItemSpace = totalItemSpace / BUTTON_COUNT;
            const float totalPaddingSpace = halfScreenHeight - totalItemSpace;
            const float individualPaddingSpace = totalPaddingSpace / (BUTTON_COUNT + 1);

            const std::vector<SDL_FRect> buttonAreas = 
                Seq::range<1, 5>()
                | Seq::map([&](const int32_t index)
                    {
                        const auto indexF = static_cast<float>(index);

                        return SDL_FRect({
                            .x = logoArea.x,
                            .y = halfScreenHeight + indexF * individualPaddingSpace + (indexF - 1) * individualItemSpace,
                            .w = logoArea.w,
                            .h = individualItemSpace,
                        });
                    });

            using enum Utility::CustomEvents;

            return { 
                TextBubble({ 
                    .Frontend = Hexagon::init(buttonAreas[0], true, "New Game"),
                    .Backend = Invokable(Utility::requestUserEvent, SDL_UserEvent(EVENT_CHANGE_SCENE_INGAME))
                }),
                
                TextBubble({ 
                    .Frontend = Hexagon::init(buttonAreas[1], true, "How to Play"),
                    .Backend = Invokable()
                }),
                
                TextBubble({ 
                    .Frontend = Hexagon::init(buttonAreas[2], true, "Settings"),
                    .Backend = Invokable()
                }),
                
                TextBubble({ 
                    .Frontend = Hexagon::init(buttonAreas[3], true, "Quit"),
                    .Backend = Invokable(Utility::requestEvent, SDL_Event(SDL_EVENT_QUIT))
                })
            };
        }

    public:
        void init()
        {
            sceneVao = OpenGL::generateAndApplyVertexArray();
            logoArea = initializeLogoArea();
            uiButtons = initializeButtons(logoArea);

            Asset::queueToLoad(Logo);
            Asset::queueToLoad(Background);
            Asset::beginLoadProcess();

            Utility::invalidate();
        }

        void deinit()
        {
            //Mix_FreeChunk(firstStartSfx);
            //SDL_DestroyTexture(backgroundImage.Resource);
            //SDL_DestroyTexture(logo.Resource);
            //SDL_DestroyTexture((*buttons[0].getLabel()).Resource);
        }

        void redraw()
        {
            OpenGL::clearScreen();

            if (sceneLoaded)
            {
                OpenGL::renderTexture(Asset::getTextureById(Background));
                OpenGL::renderTexture(Asset::getTextureById(Logo));

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
            bool anyHovered = false;

            uiButtons
            | Seq::iteri([&](const TextBubble& tb, const size_t i)
                {
                    const bool isInside = Hexagon::isCursorInside(tb.Frontend.CpuProperties.Positions, Utility::fPointToGlm(location));

                    if (isInside)
                    {
                        if (selectedButton.isNone())
                        {
                            selectedButton = Option::Some(i);
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

        void onSceneLoaded()
        {
            sceneLoaded = true;

            OpenGL::defineTextureRenderLocation(Asset::getTextureById(Logo), Option::Some(logoArea));
            OpenGL::defineTextureRenderLocation(Asset::getTextureById(Background));

            // uiButtons
            // | Seq::iter([](TextBubble& tb)
            //     {
            //         tb.Frontend.RenderProperties.TextVisible = true;
            //         tb.Frontend.RenderProperties.ButtonColor = Color::NBLACK;
            //     });

            Utility::invalidate();
        }

        void clicks()
        {
            if (selectedButton.isSome())
            {
                uiButtons.at(selectedButton.value()).Backend.execute();
            }
        }
    };
}
