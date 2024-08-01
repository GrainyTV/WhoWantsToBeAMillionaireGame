#pragma once
#include "SDL3/SDL.h"
#include "SDL3_mixer/SDL_mixer.h"
#include "textbubble.hpp"
#include <cstdint>

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

struct SceneDeinitialize
{
    template<typename Scene>
    void operator()(const Scene& scene) const
    {
        scene.deinit();
    }
};

static constexpr uint32_t BUTTON_COUNT = 4;

class MainMenuScene
{
private:
    uint32_t halfScreenHeight;
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
};

class InGameScene
{
private:

public:
    void init();

    void deinit() const;

    void redraw() const;

    void intersects(SDL_FPoint&& location);
};