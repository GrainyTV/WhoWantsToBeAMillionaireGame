#pragma once

#include "SDL3/SDL.h"
#include "optionwrap.hpp"

struct TextureRegion
{
    SDL_Texture* Resource;
    Option::Inst<SDL_FRect> Area;

    explicit TextureRegion()
        : Resource(nullptr)
        , Area(Option::None<SDL_FRect>())
    {}

    explicit TextureRegion(SDL_FRect area)
        : Resource(nullptr)
        , Area(Option::Some(area))
    {}
};
