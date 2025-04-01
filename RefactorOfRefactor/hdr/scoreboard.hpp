#pragma once
#include "opengl.hpp"
#include "roundedrectangle.hpp"
#include "ellipse.hpp"

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
}

