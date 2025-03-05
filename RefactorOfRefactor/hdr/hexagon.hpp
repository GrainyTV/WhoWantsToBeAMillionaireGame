#pragma once
#include "SDL3/SDL.h"
#include "record.hpp"
#include "svl/SVL.h"
#include <span>
#include <string_view>

namespace Hexagon
{
    HexagonInstance init(SDL_FRect, bool, std::string_view = "");

    void lateinit(HexagonInstance&, std::string_view);

    void draw(const HexagonGpu&, bool);

    bool isCursorInside(std::span<const Vec2>, Vec2);
}
