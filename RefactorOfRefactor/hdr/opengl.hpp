#pragma once
#include "SDL3/SDL.h"

namespace OpenGL
{
    void bindTexture(SDL_Texture* texture);

    void applyTextureFiltering(SDL_ScaleMode filter);
}