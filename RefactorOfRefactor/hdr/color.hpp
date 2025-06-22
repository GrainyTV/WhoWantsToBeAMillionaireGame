#pragma once
#include "SDL3/SDL.h"
#include <limits>

namespace Color
{
    namespace _impl_details
    {
        constexpr SDL_Color BLACK(24, 24, 24, 255);
        constexpr SDL_Color ORANGE(254, 125, 5, 255);
        constexpr SDL_Color BLUE(95, 194, 253, 255);
        constexpr SDL_Color GREEN(14, 146, 19, 255);
        constexpr SDL_Color RED(219, 24, 73, 255);
        //constexpr SDL_Color GRAY(232, 231, 227, 255);
        constexpr SDL_Color GRAY(172, 171, 170, 255);

        consteval SDL_FColor normalize(const SDL_Color color)
        {
            constexpr float max = std::numeric_limits<uint8_t>::max();
            SDL_FColor normalized(color.r, color.g, color.b, color.a);

            normalized.r /= max;
            normalized.g /= max;
            normalized.b /= max;
            normalized.a /= max;

            return normalized;
        }
    }
    
    constexpr SDL_Color WHITE(255, 255, 255, 255);
    constexpr SDL_FColor NBLACK = _impl_details::normalize(_impl_details::BLACK);
    constexpr SDL_FColor NORANGE = _impl_details::normalize(_impl_details::ORANGE);
    constexpr SDL_FColor NBLUE = _impl_details::normalize(_impl_details::BLUE);
    constexpr SDL_FColor NGREEN = _impl_details::normalize(_impl_details::GREEN);
    constexpr SDL_FColor NRED = _impl_details::normalize(_impl_details::RED);
    constexpr SDL_FColor NGRAY = _impl_details::normalize(_impl_details::GRAY);
}
