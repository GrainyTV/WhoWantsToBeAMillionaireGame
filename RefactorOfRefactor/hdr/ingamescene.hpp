#pragma once
#include "SDL3/SDL.h"
#include "asset.hpp"
#include "optionwrap.hpp"
#include "textbubble.hpp"
#include <array>

class InGameScene
{
using enum Asset::Identifier;
private:
    static constexpr std::array ASSETS = { Background, MusicEasy, MusicMedium, MusicHard };
    static constexpr size_t BUTTON_COUNT = 5;
    bool sceneEnabled;
    Option::Inst<int32_t> selectedButton;
    std::array<TextBubble, BUTTON_COUNT> uiButtons;

    std::array<TextBubble, BUTTON_COUNT> initializeButtons();

public:
    InGameScene();

    void deinit() const;

    void redraw() const;

    void intersects(SDL_FPoint);

    void enable();

    void clicks();
};
