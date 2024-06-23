#pragma once
#include "SDL3/SDL.h"
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

class MainMenuScene
{
private:
    uint32_t halfScreenHeight;
    bool sceneLoaded;
    TextureRegion backgroundImage;
    TextureRegion logo;
    TextBubble newGame;
    TextBubble howToPlay;
    TextBubble settings;
    TextBubble quit;
    TextBubble* selectedButton;

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