#pragma once
#include "SDL3/SDL.h"
#include "textbubble.hpp"
#include <cstdint>
#include <optional>

struct TextureRegion
{
    SDL_Texture* Resource;
    std::optional<SDL_FRect> Area;

    const SDL_FRect* getArea() const
    {
        return Area.has_value() ? &Area.value() : NULL;
    }
};

struct SceneRedrawer
{
    template<typename Scene>
    void operator()(const Scene& scene) const
    {
        scene.redraw();
    }
};

class MainMenuScene
{
private:
    static constexpr uint32_t buttonCount = 4;
    uint32_t halfScreenHeight;
    TextureRegion backgroundImage;
    TextureRegion logo;
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