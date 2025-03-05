#pragma once
#include "SDL3/SDL.h"

struct InGameScene
{
    InGameScene() = default;

    void init();

    void deinit();

    void redraw();

    void intersects(SDL_FPoint);

    void onSceneLoaded();

    void clicks();
};
