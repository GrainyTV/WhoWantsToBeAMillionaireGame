#pragma once

#include "SDL3/SDL.h"
#include "optionwrap.hpp"
#include <cstdint>

namespace Globals
{
    struct GameProperties
    {
        uint32_t ScreenWidth;
        uint32_t ScreenHeight;
        SDL_Window* Window;
        SDL_GPUDevice* Gpu;
    };

    inline auto Properties = OptionExtensions::None<GameProperties>();
}
