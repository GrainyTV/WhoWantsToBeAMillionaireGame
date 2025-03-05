#pragma once
#include "SDL3/SDL.h"

struct MainMenuScene
{
    MainMenuScene() = default;

    void init();

    void deinit();

    void redraw();

    void intersects(SDL_FPoint);

    void onSceneLoaded();

    void clicks();
};
