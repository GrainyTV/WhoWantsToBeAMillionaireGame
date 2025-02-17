#pragma once
#include "textbubble.hpp"
#include "asset.hpp"
#include <array>
#include <cstdint>

class MainMenuScene
{
using enum Asset::Identifier;
private:
    bool sceneLoaded;
    Option::Inst<int32_t> selectedButton;
    SDL_FRect logoArea;
    std::array<TextBubble, 4> uiButtons;
    static constexpr std::array assets = { Logo, Background };

public:
    MainMenuScene();

    void deinit() const;

    void redraw() const;

    void intersects(SDL_FPoint);

    void enable();

    void clicks();
};
