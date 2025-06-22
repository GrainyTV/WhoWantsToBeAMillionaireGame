#pragma once
#include "asset.hpp"
#include "opengl.hpp"
#include "ellipse.hpp"
#include "roundedrectangle.hpp"
#include <string_view>

namespace ScoreboardModule
{    
    struct Scoreboard
    {
        RoundedRectangle::RoundedRectangleInstance Rect;
        Ellipse::EllipseButton FiftyFiftyHelp;
        Ellipse::EllipseButton PhoneHelp;
        Ellipse::EllipseButton AudienceHelp;
    };

    Scoreboard init(SDL_FRect, float);

    void draw(const Scoreboard&);

    void lateinit(Ellipse::EllipseButton&, Asset::Identifier);

    void lateinit(Ellipse::EllipseButton&, std::string_view);
}

