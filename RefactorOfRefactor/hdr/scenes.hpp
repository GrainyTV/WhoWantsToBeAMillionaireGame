#pragma once
#include "SDL3/SDL.h"
#include "SDL3/SDL_rect.h"
#include "SDL3/SDL_render.h"
#include "textbubble.hpp"
#include <array>
#include <cstdint>
#include <string>

template<typename T>
struct Drawable
{
    T Resource;
    SDL_FRect Area;
};

struct SceneRedrawer
{
    template<typename T>
    void operator()(const T& scene) const
    {
        scene.redraw();
    }
};

class MainMenuScene
{
private:
    static constexpr uint32_t buttonCount = 4;
    uint32_t halfScreenHeight;
    SDL_Texture* backgroundImage;
    Drawable<SDL_Texture*> logo;
    TextBubble newGame;
    TextBubble howToPlay;
    TextBubble settings;
    TextBubble quit;

private:
    SDL_FRect initializeLogo();

    SDL_FRect retrievePositionOfButton(const uint32_t index) const;

public:
    MainMenuScene();

    ~MainMenuScene();

    void redraw() const;
};

class InGameScene
{
private:

public:
    void init();

    void deinit();

    void redraw() const;
};