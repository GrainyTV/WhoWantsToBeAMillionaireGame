#pragma once

#include "SDL3/SDL_render.h"
#include "SDL3/SDL_rect.h"
#include "option.hpp"

struct TextureRegion
{
    SDL_Texture* Resource; // = nullptr;
    Option<SDL_FRect> Area; // = Option<SDL_FRect>::None();

    const SDL_FRect* getArea() const
    {
        return Area.isSome() ? &Area.value() : nullptr;
    }

    TextureRegion()
        : Resource(nullptr)
        , Area(Option<SDL_FRect>::None())
    {}

    TextureRegion(const SDL_FRect& area)
        : Resource(nullptr)
        , Area(Option<SDL_FRect>::Some(area))
    {}

    TextureRegion(const TextureRegion& other)
        : Resource(other.Resource)
        , Area(other.Area)
    {}
};
