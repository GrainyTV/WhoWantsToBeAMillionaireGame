#pragma once
#include "SDL3/SDL.h"

class InGameScene
{
private:

public:
    InGameScene();

    void deinit() const;

    void redraw() const;

    void intersects(SDL_FPoint&& location);

    void enable();

    void clicks();
};