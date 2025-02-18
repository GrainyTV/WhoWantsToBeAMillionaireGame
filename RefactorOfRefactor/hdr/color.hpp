#pragma once
#include "SDL3/SDL.h"

namespace Color
{
    static consteval SDL_FColor normalize(const SDL_Color& color)
    {
        constexpr float max = 255.0f;

        return {
            static_cast<float>(color.r) / max,
            static_cast<float>(color.g) / max,
            static_cast<float>(color.b) / max,
            static_cast<float>(color.a) / max,
        };
    }

    constexpr SDL_Color BLACK{ .r = 33, .g = 33, .b = 33, .a = 255 };
    constexpr SDL_Color WHITE{ .r = 255, .g = 255, .b = 255, .a = 255 };
    constexpr SDL_Color ORANGE{ .r = 254, .g = 125, .b = 5, .a = 255 };
    constexpr SDL_Color BLUE{ .r = 95, .g = 194, .b = 253, .a = 255 };
    constexpr SDL_Color GREEN{ .r = 14, .g = 146, .b = 19, .a = 255 };

    constexpr SDL_FColor NBLACK = normalize(BLACK);
    constexpr SDL_FColor NORANGE = normalize(ORANGE);
    constexpr SDL_FColor NBLUE = normalize(BLUE);
}
