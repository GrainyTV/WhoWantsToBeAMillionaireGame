#pragma once
#include "SDL3/SDL.h"
#include "event.hpp"
#include "assert.hpp"

struct Game
{
    static inline SDL_Window* window{ NULL };
    static inline SDL_Renderer* renderer{ NULL };
    static inline Event eventHandler{};

    static void init()
    {
        const auto initialization = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS);
        ASSERT(initialization == 0, "Failed to initialize SDL with {}", SDL_GetError());

        window = SDL_CreateWindow("Who Wants To Be A Millionaire?", 1920, 1080, SDL_WINDOW_FULLSCREEN | SDL_WINDOW_VULKAN);
        ASSERT(window != NULL, "SDL_CreateWindow() failed with {}", SDL_GetError());

        renderer = SDL_CreateRenderer(window, NULL, SDL_RENDERER_ACCELERATED | SDL_RENDERER_SOFTWARE);
        ASSERT(renderer != NULL, "SDL_CreateRenderer() failed with {}", SDL_GetError());
    }

    static void deinit()
    {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }

    static void launch()
    {
        eventHandler.processIncomingEvents();
    }
};