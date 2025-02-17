#pragma once
#include "invokable.hpp"
#include "hexagon.hpp"

struct TextBubble
{
    Hexagon Frontend;
    Invokable Backend;

    SDL_FRect getArea() const
    {
        return { 0, 0, 100, 100 };
    }

    std::string getText() const
    {
        return "not yet implemented";
    }
};

