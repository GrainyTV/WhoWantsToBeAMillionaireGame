#pragma once
#include "SDL3/SDL.h"
#include "shader.hpp"
#include <string_view>

namespace FontManager
{
    void init();

    Shader::TextureInfo generateFromText(std::string_view, float, float);

    SDL_FRect alignRenderedTextToCenter(const Shader::TextureInfo&, SDL_FRect);
}
