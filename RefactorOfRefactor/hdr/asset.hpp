#pragma once
#include "SDL3/SDL.h"

namespace Asset
{
    enum class Identifier
    {
        Logo,
        Background,
    };

    void queueToLoad(const Identifier);

    void beginLoadProcess();

    SDL_Texture* getTextureById(const Identifier);
}
