#pragma once
#include "SDL3_image/SDL_image.h"
#include "assert.hpp"
#include "event.hpp"
#include "extensions.hpp"
#include "texture.hpp"
#include <cstdint>

struct Game
{
    static inline uint32_t ScreenWidth{ 0 };
    static inline uint32_t ScreenHeight{ 0 };
    static inline SDL_Window* Window{ NULL };
    static inline SDL_Renderer* Renderer{ NULL };
    static inline Texture TextureLoader{};
    static inline Event EventHandler{};

    static void init()
    {
        {
            const auto init = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS);
            ASSERT(init == 0, "Failed to initialize SDL ({})", SDL_GetError());
        }

        {
            const auto init = IMG_Init(IMG_INIT_PNG);
            ASSERT(init == IMG_INIT_PNG, "Failed to initialize SDL_image ({})", IMG_GetError());
        }

        const auto msaa8 = SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 8);
        ASSERT(msaa8 == 0, "Failed to enable multisample anti-aliasing x8 ({})", SDL_GetError());

        Window = SDL_CreateWindow("Who Wants To Be A Millionaire?", ScreenWidth, ScreenHeight, SDL_WINDOW_OPENGL | SDL_WINDOW_HIDDEN);
        ASSERT(Window != NULL, "SDL_CreateWindow() failed ({})", SDL_GetError());

        const auto screenProperties = ext::displayInfo(Window);
        ScreenWidth = (*screenProperties).w;
        ScreenHeight = (*screenProperties).h;

        SDL_SetWindowSize(Window, ScreenWidth, ScreenHeight);
        SDL_SetWindowFullscreen(Window, true);
        SDL_SyncWindow(Window);
        SDL_ShowWindow(Window);

        Renderer = SDL_CreateRenderer(Window, NULL, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        ASSERT(Renderer != NULL, "SDL_CreateRenderer() failed ({})", SDL_GetError());
    }

    static void deinit()
    {
        SDL_DestroyRenderer(Renderer);
        SDL_DestroyWindow(Window);
        IMG_Quit();
        SDL_Quit();
    }

    static void launch()
    {
        EventHandler.applyDefaultScene();
        EventHandler.processIncomingEvents();
    }
};