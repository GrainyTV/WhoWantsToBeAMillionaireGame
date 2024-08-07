#pragma once
#include "SDL3/SDL.h"
#include "textbubble.hpp"

class InGameScene
{
private:
    static constexpr uint32_t BUTTON_COUNT = 4;
    bool sceneLoaded;
    std::array<TextBubble, BUTTON_COUNT> buttons;

private:
    SDL_FRect retrievePositionOfButton(const uint32_t index);

public:
    InGameScene();

    void deinit() const;

    void redraw() const;

    void intersects(const SDL_FPoint location);

    void enable();

    void clicks();
};