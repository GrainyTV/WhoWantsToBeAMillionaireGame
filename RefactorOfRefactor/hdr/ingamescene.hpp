#pragma once
#include "SDL3/SDL.h"

struct InGameScene
{
    InGameScene();

    void deinit() const;

    void redraw() const;

    void intersects(const SDL_FPoint);

    void enable();

    void clicks();
};
