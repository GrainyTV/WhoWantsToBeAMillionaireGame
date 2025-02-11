#pragma once
#include "SDL3/SDL.h"

namespace OpenGL
{
    void bindTexture(SDL_Texture*);

    void applyTextureFiltering(SDL_ScaleMode);
}
