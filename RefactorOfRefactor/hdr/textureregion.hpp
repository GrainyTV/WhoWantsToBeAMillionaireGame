#pragma once

#include "SDL3/SDL.h"
#include "option.hpp"

struct TextureRegion
{
    SDL_Texture* Resource;
    Option<SDL_FRect> Area;

    TextureRegion()
        : Resource(nullptr)
        , Area(Option<SDL_FRect>::None())
    {}
};
