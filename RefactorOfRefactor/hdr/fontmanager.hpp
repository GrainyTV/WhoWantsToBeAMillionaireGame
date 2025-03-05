#pragma once
#include "SDL3/SDL.h"
#include "record.hpp"
#include <string_view>

namespace FontManager
{
    void init();

    TextureGpu generateFromText(std::string_view, float, float);

    SDL_FRect centerInsideArea(TextureGpu, SDL_FRect);
}
