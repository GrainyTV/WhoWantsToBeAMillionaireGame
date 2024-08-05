#pragma once
#include "SDL3/SDL.h"

struct SceneRedrawer
{
    template<typename Scene>
    void operator()(const Scene& scene) const
    {
        scene.redraw();
    }
};

struct IntersectsUiElement
{
    float x;
    float y;

    template<typename Scene>
    void operator()(Scene& scene)
    {
        scene.intersects(SDL_FPoint{ x, y });
    }
};

struct ClicksUiElement
{
    template<typename Scene>
    void operator()(Scene& scene)
    {
        scene.clicks();
    }
};

struct SceneDeinitialize
{
    template<typename Scene>
    void operator()(const Scene& scene) const
    {
        scene.deinit();
    }
};