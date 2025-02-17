#pragma once
#include "SDL3/SDL.h"
#include <cstdint>

namespace Asset
{
    enum class Identifier : std::uint8_t
    {
        Logo,
        Background,
    };

    void queueToLoad(Identifier);

    void beginLoadProcess();

    SDL_Texture* getTextureById(Identifier);
}
