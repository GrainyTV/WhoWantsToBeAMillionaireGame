#pragma once
#include "SDL3_mixer/SDL_mixer.h"
#include "textbubble.hpp"
#include <cstdint>

static constexpr uint32_t BUTTON_COUNT = 4;

class MainMenuScene
{
private:
    bool sceneLoaded;
    TextureRegion backgroundImage;
    TextureRegion logo;
    std::array<TextBubble, BUTTON_COUNT> buttons;
    TextBubble* selectedButton;
    Mix_Chunk* firstStartSfx;

private:
    SDL_FRect initializeLogo();

    SDL_FRect retrievePositionOfButton(const uint32_t index) const;

public:
    MainMenuScene();

    void deinit() const;

    void redraw() const;

    void intersects(SDL_FPoint&& location);

    void enable();

    void clicks();
};
