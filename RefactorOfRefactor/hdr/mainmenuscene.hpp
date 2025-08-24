#pragma once
#include "iscene.hpp"
#include "asset.hpp"
#include "globals.hpp"
#include "textbubble.hpp"
#include "optionwrap.hpp"
#include "seq/seq.hpp"
#include "utility.hpp"

namespace Scene
{
    using TextBubbleModule::TextBubble;
    using enum Utility::CustomEvents;
    using enum Asset::Identifier;

    class MainMenuScene
    {
    private:
        bool sceneLoaded = false;
        
        Option<size_t> selectedButton = OptionExtensions::None<size_t>();

        SDL_FRect logoArea = initializeLogoArea();

        static constexpr std::size_t BUTTON_COUNT = 4;
        std::array<TextBubble, BUTTON_COUNT> uiButtons = initializeButtons(logoArea);

        static SDL_FRect initializeLogoArea()
        {
            const auto& properties = Globals::Properties.value();
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
            const auto& properties = Globals::Properties.value();
            const auto screenHeight = static_cast<float>(properties.ScreenHeight);

            const float halfScreenHeight = screenHeight / 2;
            const float totalItemSpace = halfScreenHeight * 0.7f;
            const float individualItemSpace = totalItemSpace / BUTTON_COUNT;
            const float totalPaddingSpace = halfScreenHeight - totalItemSpace;
            const float individualPaddingSpace = totalPaddingSpace / (BUTTON_COUNT + 1);

            const std::array<SDL_FRect, 4> buttonAreas = 
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
                TextBubble(buttonAreas[0], OptionExtensions::Some(Invokable(Utility::requestUserEvent, SDL_UserEvent(EVENT_CHANGE_SCENE_INGAME)))),
                TextBubble(buttonAreas[1]),
                TextBubble(buttonAreas[2]),
                TextBubble(buttonAreas[3], OptionExtensions::Some(Invokable(Utility::requestEvent, SDL_Event(SDL_EVENT_QUIT)))),
                //TextBubble({ 
                //    .Frontend = Hexagon::init(buttonAreas[0], true, "New Game"),
                //    .Backend = Invokable(Utility::requestUserEvent, SDL_UserEvent(EVENT_CHANGE_SCENE_INGAME))
                //}),
                //
                //TextBubble({ 
                //    .Frontend = Hexagon::init(buttonAreas[1], true, "How to Play"),
                //    .Backend = Invokable()
                //}),
                //
                //TextBubble({ 
                //    .Frontend = Hexagon::init(buttonAreas[2], true, "Settings"),
                //    .Backend = Invokable()
                //}),
                //
                //TextBubble({ 
                //    .Frontend = Hexagon::init(buttonAreas[3], true, "Quit"),
                //    .Backend = Invokable(Utility::requestEvent, SDL_Event(SDL_EVENT_QUIT))
                //})
            };
        }

    public:
        void init()
        {
            Asset::queueToLoad(Logo);
            Asset::queueToLoad(Background);
            Asset::beginLoadProcess();

            uiButtons[0].applyText("New Game");
            uiButtons[1].applyText("How to Play");
            uiButtons[2].applyText("Settings");
            uiButtons[3].applyText("Quit");

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
            const auto& properties = Globals::Properties.value();
            SDL_GPUCommandBuffer* const commandBuffer = SDL_AcquireGPUCommandBuffer(properties.Gpu);

            if (commandBuffer != nullptr)
            {
                SDL_GPUTexture* swapchainTexture = nullptr;
                
                if (SDL_WaitAndAcquireGPUSwapchainTexture(commandBuffer, properties.Window, &swapchainTexture, nullptr, nullptr))
                {
                    SDL_GPURenderPass* const render = Shader::beginRender(commandBuffer, swapchainTexture);

                    if (sceneLoaded)
                    {
                        Shader::renderTexture(commandBuffer, render, Asset::getTextureById(Background));
                        Shader::renderTexture(commandBuffer, render, Asset::getTextureById(Logo));

                        uiButtons
                        | Seq::iter([&](const TextBubble& tb)
                            {
                                tb.draw(commandBuffer, render);
                            });
                    }

                    SDL_EndGPURenderPass(render);
                    Shader::finalizeRender(commandBuffer, swapchainTexture);
                }
            }

            if (!SDL_SubmitGPUCommandBuffer(commandBuffer))
            {
                Debug::log("You done fucked up!");
            }
        }

        void intersects(const SDL_FPoint location)
        {
            bool anyHovered = false;

            uiButtons
            | Seq::iteri([&](size_t i, const TextBubble& tb)
                {
                    if (Utility::danSundayWindingNumberCheck(tb.outerPoints(), Utility::fPointToGlm(location)))
                    {
                        if (selectedButton.isNone())
                        {
                            uiButtons.at(i).changeBackgroundColorTo(Color::NORANGE);
                            selectedButton = OptionExtensions::Some(i);
                            Utility::invalidate();
                        }

                        anyHovered = true;
                    }
                });

            if (anyHovered == false && selectedButton.isSome())
            {
                uiButtons.at(selectedButton.value()).changeBackgroundColorTo(Color::NBLACK);
                selectedButton = OptionExtensions::None<size_t>();
                Utility::invalidate();
            }
        }

        void onSceneLoaded()
        {
            sceneLoaded = true;
            const auto& properties = Globals::Properties.value();

            Shader::beginUploadToGpu();
            Shader::uploadTextureToGpuWithAreaIfNeeded(Asset::getTextureById(Background));
            Shader::uploadTextureToGpuWithAreaIfNeeded(Asset::getTextureById(Logo), OptionExtensions::Some(logoArea));
            uiButtons
            | Seq::iter([](const TextBubble& tb)
                {
                    Shader::uploadTextureToGpuWithAreaIfNeeded(tb.renderedText(), OptionExtensions::Some(tb.whereToRenderTextAt())); 
                });
            Shader::endUploadToGpu();

            SDL_GPUCommandBuffer* const commandBuffer = SDL_AcquireGPUCommandBuffer(properties.Gpu);
            SDL_GenerateMipmapsForGPUTexture(commandBuffer, Asset::getTextureById(Logo).Texture);
            SDL_SubmitGPUCommandBuffer(commandBuffer);

            Utility::invalidate();
        }

        void clicks()
        {
            if (selectedButton.isSome())
            {
                uiButtons.at(selectedButton.value()).click();
            }
        }
    };
}
