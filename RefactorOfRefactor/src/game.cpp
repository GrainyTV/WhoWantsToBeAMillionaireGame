#include "game.hpp"
#include "SDL3_image/SDL_image.h"
#include "SDL3_mixer/SDL_mixer.h"
#include "SDL3_ttf/SDL_ttf.h"
#include "event.hpp"
#include "globals.hpp"
#include "option.hpp"
#include "result.hpp"
#include "utility.hpp"
#include <cstdint>

using Globals::GameProperties;

void Game::init()
{
    const auto sdlInit = Result<int32_t>(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS), "Failed to initialize SDL");
    sdlInit.assertOk();

    const auto sdlImageInit = Result<int32_t>(IMG_Init(IMG_INIT_PNG), "Failed to initialize SDL_image", IMG_INIT_PNG);
    sdlImageInit.assertOk();

    const auto sdlAudioInit = Result<int32_t>(Mix_Init(MIX_INIT_MP3), "Failed to initialize SDL_mixer", MIX_INIT_MP3);
    sdlAudioInit.assertOk();

    const auto sdlTextInit = Result<int32_t>(TTF_Init(), "Failed to initialize SDL_ttf");
    sdlTextInit.assertOk();

    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 8);

    const auto windowInit = Result<SDL_Window*>(
        SDL_CreateWindow("Who Wants To Be A Millionaire?", 0, 0, SDL_WINDOW_OPENGL),
        "SDL_CreateWindow() failed");
    windowInit.assertOk();

    SDL_Window* const window = windowInit.value();

    const SDL_DisplayMode* screenProperties = Utility::displayInfo(window);
    const int32_t screenWidth = (*screenProperties).w;
    const int32_t screenHeight = (*screenProperties).h;

    const auto rendererInit = Result<SDL_Renderer*>(
        SDL_CreateRenderer(window, nullptr, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC),
        "SDL_CreateRenderer() failed");
    rendererInit.assertOk();

    SDL_Renderer* const renderer = rendererInit.value();

    Globals::Properties = Option<GameProperties>::Some({
        .ScreenWidth = screenWidth,
        .ScreenHeight = screenHeight,
        .Window = window,
        .Renderer = renderer,
    });
}

void Game::deinit()
{
    const GameProperties properties = Globals::Properties.value();

    SDL_DestroyRenderer(properties.Renderer);
    SDL_DestroyWindow(properties.Window);
    TTF_Quit();
    Mix_Quit();
    IMG_Quit();
    SDL_Quit();
}

void Game::launch()
{
    Event::applyDefaultSettings();
    Event::processIncomingEvents();
}