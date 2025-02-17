#pragma once
#include "SDL3/SDL.h"
#include <string_view>

namespace FontManager
{
    void init();

    SDL_Texture* generateFromText(std::string_view, std::pair<float, float>);

    SDL_FRect centerInsideArea(SDL_Texture*, SDL_FRect);
}
