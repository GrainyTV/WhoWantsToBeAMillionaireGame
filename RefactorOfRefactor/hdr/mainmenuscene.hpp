#pragma once
#include "textbubble.hpp"
#include "asset.hpp"
#include <array>
#include <cstdint>

class MainMenuScene
{
using enum Asset::Identifier;
private:
    static constexpr std::array ASSETS = { Logo, Background };
    static constexpr size_t BUTTON_COUNT = 4;
    bool sceneLoaded;
    Option::Inst<int32_t> selectedButton;
    SDL_FRect logoArea;
    std::array<TextBubble, BUTTON_COUNT> uiButtons;

public:
    MainMenuScene();

    void deinit() const;

    void redraw() const;

    void intersects(SDL_FPoint);

    void enable();

    void clicks();
};
