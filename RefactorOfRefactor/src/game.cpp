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

#include "glad/gl.h"

void Game::init()
{
    // ┏━━━━━━━━━━┓
    // ┃ SDL Init ┃
    // ┗━━━━━━━━━━┛

    SDL_SetHint(SDL_HINT_VIDEO_DOUBLE_BUFFER, "true");

    const auto sdlInit = Result(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS), "Failed to initialize SDL");
    assert(sdlInit.isOk(), "{}", sdlInit.toString());

    // ┏━━━━━━━━━━━━┓
    // ┃ Audio Init ┃
    // ┗━━━━━━━━━━━━┛

    const auto sdlAudioInit = Result(Mix_Init(MIX_INIT_MP3), "Failed to initialize SDL_mixer", static_cast<uint32_t>(MIX_INIT_MP3));
    assert(sdlAudioInit.isOk(), "{}", sdlAudioInit.toString());

    const auto audioDeviceInit = Result(Mix_OpenAudio(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, nullptr), "Failed to open default audio device for playback");
    assert(audioDeviceInit.isOk(), "{}", audioDeviceInit.toString());

    // ┏━━━━━━━━━━━┓
    // ┃ Text Init ┃
    // ┗━━━━━━━━━━━┛

    const auto sdlTextInit = Result(TTF_Init(), "Failed to initialize SDL_ttf");
    assert(sdlTextInit.isOk(), "{}", sdlTextInit.toString());

    // ┏━━━━━━━━━━━━━━┓
    // ┃ General Init ┃
    // ┗━━━━━━━━━━━━━━┛

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 8);
    SDL_GL_SetSwapInterval(1);

    const auto windowInit = Result(
        SDL_CreateWindow("Who Wants To Be A Millionaire?", 0, 0, SDL_WINDOW_OPENGL),
        "SDL_CreateWindow() failed");
    assert(windowInit.isOk(), "{}", windowInit.toString());

    SDL_Window* const window = windowInit.unwrap();
    //SDL_GLContext glContext = SDL_GL_CreateContext(window);
    SDL_GL_CreateContext(window);
    int version = gladLoadGL(static_cast<GLADloadfunc>(SDL_GL_GetProcAddress));
    printf("GL %d.%d\n", GLAD_VERSION_MAJOR(version), GLAD_VERSION_MINOR(version));

    const SDL_DisplayMode* screenProperties = Utility::displayInfo(window);

    glViewport(0, 0, (*screenProperties).w, (*screenProperties).h);

    // SDL_PropertiesID rendererProperties = SDL_CreateProperties();
    // SDL_SetStringProperty(rendererProperties, SDL_PROP_RENDERER_CREATE_NAME_STRING, "opengl");
    // SDL_SetPointerProperty(rendererProperties, SDL_PROP_RENDERER_CREATE_WINDOW_POINTER, window);
    // SDL_SetNumberProperty(rendererProperties, SDL_PROP_RENDERER_CREATE_PRESENT_VSYNC_NUMBER, 1);

    // const auto rendererInit = Result(
    //     SDL_CreateRendererWithProperties(rendererProperties),
    //     "SDL_CreateRenderer() failed");
    // assert(rendererInit.isOk(), "{}", rendererInit.toString());

    // SDL_Renderer* const renderer = rendererInit.unwrap();

    Globals::Properties = Option::Some<Globals::GameProperties>({
        .ScreenWidth = (*screenProperties).w,
        .ScreenHeight = (*screenProperties).h,
        .Window = window,
        //.Renderer = renderer,
    });
}

void Game::deinit()
{
    const auto properties = Globals::Properties.value();

    //SDL_DestroyRenderer(properties.Renderer);
    SDL_DestroyWindow(properties.Window);
    TTF_Quit();
    Mix_Quit();
    SDL_Quit();
}

void Game::launch()
{
    Event::startRunningOnMainUiThread();
}
