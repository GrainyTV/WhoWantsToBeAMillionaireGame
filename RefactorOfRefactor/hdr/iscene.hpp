#pragma once
#include "SDL3/SDL_rect.h"

struct IScene
{
    virtual ~IScene() = default;

    virtual void deinit() const = 0;

    virtual void redraw() const = 0;

    virtual void intersects(const SDL_FPoint) = 0;

    virtual void clicks() = 0;

    virtual void enable() = 0;
};
