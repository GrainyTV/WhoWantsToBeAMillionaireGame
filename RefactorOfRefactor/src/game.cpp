#include "game.hpp"
#include "SDL3_image/SDL_image.h"
#include "SDL3_mixer/SDL_mixer.h"
#include "SDL3_ttf/SDL_ttf.h"
#include "debug.hpp"
#include "event.hpp"
#include "globals.hpp"
#include "optionwrap.hpp"
#include "result.hpp"
#include "utility.hpp"
#include <cstdint>

void Game::init()
{
    // ┏━━━━━━━━━━┓
    // ┃ SDL Init ┃
    // ┗━━━━━━━━━━┛

    SDL_SetHint(SDL_HINT_VIDEO_DOUBLE_BUFFER, "true");

    const auto sdlInit = Result(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS), "Failed to initialize SDL");
    Debug::assert(sdlInit.isOk(), "{} SDL({})", sdlInit.toString(), SDL_GetError());

    // ┏━━━━━━━━━━━━┓
    // ┃ Audio Init ┃
    // ┗━━━━━━━━━━━━┛

    const auto sdlAudioInit = Result(Mix_Init(MIX_INIT_MP3), "Failed to initialize SDL_mixer", static_cast<uint32_t>(MIX_INIT_MP3));
    Debug::assert(sdlAudioInit.isOk(), "{} SDL({})", sdlAudioInit.toString(), SDL_GetError());

    const auto audioDeviceInit = Result(Mix_OpenAudio(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, nullptr), "Failed to open default audio device for playback");
    Debug::assert(audioDeviceInit.isOk(), "{} SDL({})", audioDeviceInit.toString(), SDL_GetError());

    // ┏━━━━━━━━━━━┓
    // ┃ Text Init ┃
    // ┗━━━━━━━━━━━┛

    const auto sdlTextInit = Result(TTF_Init(), "Failed to initialize SDL_ttf");
    Debug::assert(sdlTextInit.isOk(), "{} SDL({})", sdlTextInit.toString(), SDL_GetError());

    // ┏━━━━━━━━━━━━━━┓
    // ┃ General Init ┃
    // ┗━━━━━━━━━━━━━━┛

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 8);
    SDL_GL_SetSwapInterval(1);

    const auto windowInit = Result(
        SDL_CreateWindow("Who Wants To Be A Millionaire?", 0, 0, SDL_WINDOW_OPENGL),
        "SDL_CreateWindow() failed");
    Debug::assert(windowInit.isOk(), "{} SDL({})", windowInit.toString(), SDL_GetError());

    SDL_Window* const window = windowInit.unwrap();
    const SDL_DisplayMode* const screenProperties = Utility::displayInfo(window);

    Globals::Properties = Option::Some<Globals::GameProperties>({
        .ScreenWidth = (*screenProperties).w,
        .ScreenHeight = (*screenProperties).h,
        .Window = window,
    });
}

void Game::deinit()
{
    const auto properties = Globals::Properties.value();

    SDL_DestroyWindow(properties.Window);
    TTF_Quit();
    Mix_Quit();
    SDL_Quit();
}

void Game::launch()
{
    Event::startRunningOnMainUiThread();
}
