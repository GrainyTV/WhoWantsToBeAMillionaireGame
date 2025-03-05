#pragma once
#include "SDL3/SDL.h"
#include <limits>

namespace Color
{
    static consteval SDL_FColor normalize(const SDL_Color color)
    {
        constexpr float max = std::numeric_limits<uint8_t>::max();
        SDL_FColor normalized(color.r, color.g, color.b, color.a);

        normalized.r /= max;
        normalized.g /= max;
        normalized.b /= max;
        normalized.a /= max;

        return normalized;
    }

    constexpr SDL_Color BLACK(33, 33, 33, 255);
    constexpr SDL_Color WHITE(255, 255, 255, 255);
    constexpr SDL_Color ORANGE(254, 125, 5, 255);
    constexpr SDL_Color BLUE(95, 194, 253, 255);
    constexpr SDL_Color GREEN(14, 146, 19, 255);

    constexpr SDL_FColor NBLACK = normalize(BLACK);
    constexpr SDL_FColor NORANGE = normalize(ORANGE);
    constexpr SDL_FColor NBLUE = normalize(BLUE);
}
