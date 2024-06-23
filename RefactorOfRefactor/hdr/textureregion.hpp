#pragma once
#include "SDL3/SDL.h"
#include <optional>

struct TextureRegion
{
    SDL_Texture* Resource;
    std::optional<SDL_FRect> Area;

    const SDL_FRect* getArea() const
    {
        return Area.has_value() ? &Area.value() : NULL;
    }
};