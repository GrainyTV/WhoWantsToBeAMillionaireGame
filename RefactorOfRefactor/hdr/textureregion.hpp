#pragma once

#include "SDL3/SDL.h"
#include "option.hpp"

struct TextureRegion
{
    SDL_Texture* Resource;
    Option::Option<SDL_FRect> Area;

    TextureRegion()
        : Resource(nullptr)
        , Area(Option::None<SDL_FRect>())
    {}

    TextureRegion(Option::Option<SDL_FRect>&& area)
        : Resource(nullptr)
        , Area(area)
    {}
};
