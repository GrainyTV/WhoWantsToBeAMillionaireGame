#include "game.hpp"
#include "SDL3_image/SDL_image.h"
#include "SDL3_mixer/SDL_mixer.h"
#include "SDL3_ttf/SDL_ttf.h"
#include "debug.hpp"
#include "event.hpp"
#include "globals.hpp"
#include "option.hpp"
#include "result.hpp"
#include "utility.hpp"
#include <cstdint>
    
using Globals::GameProperties;

void Game::init()
{
    const auto sdlInit = Result(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS), "Failed to initialize SDL");
    ASSERT(sdlInit.isOk(), "{}", sdlInit.cause());

    //const auto sdlImageInit = Result<int32_t>(IMG_Init(IMG_INIT_PNG), "Failed to initialize SDL_image", IMG_INIT_PNG);
    //ASSERT(sdlImageInit.isOk(), "{}", sdlImageInit.cause());

    const auto sdlAudioInit = Result(Mix_Init(MIX_INIT_MP3), "Failed to initialize SDL_mixer", (uint32_t) MIX_INIT_MP3);
    ASSERT(sdlAudioInit.isOk(), "{}", sdlAudioInit.cause());

    const auto sdlTextInit = Result(TTF_Init(), "Failed to initialize SDL_ttf");
    ASSERT(sdlTextInit.isOk(), "{}", sdlTextInit.cause());

    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 8);

    const auto windowInit = Result(
        SDL_CreateWindow("Who Wants To Be A Millionaire?", 0, 0, SDL_WINDOW_OPENGL),
        "SDL_CreateWindow() failed");
    ASSERT(windowInit.isOk(), "{}", windowInit.cause());

    SDL_Window* const window = windowInit.value();

    const SDL_DisplayMode* screenProperties = Utility::displayInfo(window);

    SDL_PropertiesID rendererProperties = SDL_CreateProperties();
    SDL_SetPointerProperty(rendererProperties, SDL_PROP_RENDERER_CREATE_WINDOW_POINTER, window);
    SDL_SetNumberProperty(rendererProperties, SDL_PROP_RENDERER_CREATE_PRESENT_VSYNC_NUMBER, 1);

    const auto rendererInit = Result(
        //SDL_CreateRenderer(window, nullptr, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC),
        SDL_CreateRendererWithProperties(rendererProperties),
        "SDL_CreateRenderer() failed");
    ASSERT(rendererInit.isOk(), "{}", rendererInit.cause());

    SDL_Renderer* const renderer = rendererInit.value();

    Globals::Properties = Option::Some(
        GameProperties({
            .ScreenWidth = (*screenProperties).w,
            .ScreenHeight = (*screenProperties).h,
            .Window = window,
            .Renderer = renderer,
        })
    );
}

void Game::deinit()
{
    const GameProperties properties = Globals::Properties.value();

    SDL_DestroyRenderer(properties.Renderer);
    SDL_DestroyWindow(properties.Window);
    TTF_Quit();
    Mix_Quit();
    //IMG_Quit();
    SDL_Quit();
}

void Game::launch()
{
    Event::applyDefaultSettings();
    Event::processIncomingEvents();
}
