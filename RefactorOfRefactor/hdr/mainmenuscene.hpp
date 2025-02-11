#pragma once
//#include "SDL3_mixer/SDL_mixer.h"
//#include "iscene.hpp"
#include "textbubble.hpp"
//#include "textureregion.hpp"
#include <array>
#include <cstdint>
#include "asset.hpp"

class MainMenuScene
{
using enum Asset::Identifier;
private:
    bool sceneLoaded;
    Option::Option<int32_t> selectedButton;
    SDL_FRect logoArea;
    std::array<TextBubble, 4> buttons;
   
    static constexpr Asset::Identifier assets[] = { Logo, Background };
    //Mix_Chunk* firstStartSfx;

public:
    MainMenuScene();

    void deinit() const;

    void redraw() const;

    void intersects(const SDL_FPoint);

    void enable();

    void clicks();
};
