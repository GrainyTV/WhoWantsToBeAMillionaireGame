#pragma once
#include "SDL3/SDL_rect.h"
#include "iscene.hpp"

struct InGameScene : public IScene
{
    InGameScene();

    void deinit() const;

    void redraw() const;

    void intersects(const SDL_FPoint);

    void enable();

    void clicks();
};