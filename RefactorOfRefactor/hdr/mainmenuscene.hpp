#pragma once
#include "SDL3_mixer/SDL_mixer.h"
#include "iscene.hpp"
#include "textbubble.hpp"
#include "textureregion.hpp"
#include <array>

class MainMenuScene : public IScene
{
private:
    bool sceneLoaded;
    TextureRegion logo;
    std::array<TextBubble, 4> buttons;
    TextBubble* selectedButton;
    Mix_Chunk* firstStartSfx;

public:
    MainMenuScene();

    void deinit() const;

    void redraw() const;

    void intersects(const SDL_FPoint);

    void enable();

    void clicks();
};
