#pragma once

#include "SDL3/SDL_render.h"
#include "SDL3/SDL_video.h"
#include "option.hpp"
#include <cstdint>

namespace Globals
{
    struct GameProperties
    {
        int32_t ScreenWidth;
        int32_t ScreenHeight;
        SDL_Window* Window;
        SDL_Renderer* Renderer;
    };

    inline auto Properties = Option<GameProperties>::None();
}
