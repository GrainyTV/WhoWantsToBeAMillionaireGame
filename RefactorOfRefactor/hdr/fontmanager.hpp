#pragma once
#include "SDL3/SDL.h"
#include "opengl.hpp"
#include <string_view>

namespace FontManager
{
    void init();

    OpenGL::TextureGpu generateFromText(std::string_view, float, float, bool);

    SDL_FRect centerInsideArea(OpenGL::TextureGpu, SDL_FRect);
}
