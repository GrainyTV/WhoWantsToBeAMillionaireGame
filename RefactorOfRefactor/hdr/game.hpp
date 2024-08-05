#pragma once
#include "SDL3_image/SDL_image.h"
#include "SDL3_mixer/SDL_mixer.h"
#include "debug.hpp"
#include "event.hpp"
#include "extensions.hpp"
#include "texture.hpp"
#include <cstdint>
#include "fontmanager.hpp"

struct Game
{
    static inline uint32_t ScreenWidth{ 0 };
    static inline uint32_t ScreenHeight{ 0 };
    static inline SDL_Window* Window{ NULL };
    static inline SDL_Renderer* Renderer{ NULL };
    static inline FontManager FontManager{};
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

        {
            const auto init = TTF_Init();
            ASSERT(init == 0, "Failed to initialize SDL_ttf ({})", TTF_GetError());
            
            FontManager.init();
        }

        {
            const auto init = Mix_Init(MIX_INIT_MP3);
            ASSERT(init == MIX_INIT_MP3, "Failed to initialize SDL_mixer ({})", Mix_GetError());

            const auto audioDevice = Mix_OpenAudio(SDL_AUDIO_DEVICE_DEFAULT_OUTPUT, NULL);
            ASSERT(audioDevice == 0, "Failed to select default audio playback ({})", Mix_GetError());

            Mix_Volume(-1, MIX_MAX_VOLUME / 2);
        }

        const auto msaa8 = SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 8);
        ASSERT(msaa8 == 0, "Failed to enable multisarontPagemple anti-aliasing x8 ({})", SDL_GetError());

        const auto dbusAutoShutdown = SDL_SetHint(SDL_HINT_SHUTDOWN_DBUS_ON_QUIT, "1");
        ASSERT(dbusAutoShutdown, "Failed to enable automatic DBUS shutdown");

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
        FontManager.deinit();
        SDL_DestroyRenderer(Renderer);
        SDL_DestroyWindow(Window);
        Mix_CloseAudio();
        Mix_Quit();
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
    }

    static void launch()
    {
        EventHandler.applyDefaultScene();
        EventHandler.processIncomingEvents();
    }
};