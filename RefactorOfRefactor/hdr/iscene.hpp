#pragma once
#include "SDL3/SDL.h"
#include <cstdint>
#include <variant>

enum class SceneOperation : std::uint8_t
{
    INIT,
    DEINIT,
    CLICK,
    HOVER,
    REDRAW,
    ENABLE
};

class IScene
{
private:
    SceneOperation selected;
    SDL_FPoint cursor;

public:
    explicit IScene(const SceneOperation oper)
        : selected(oper)
        , cursor(-1, -1)
    {}

    explicit IScene(const SceneOperation oper, const SDL_FPoint pos)
        : selected(oper)
        , cursor(pos)
    {}

    void operator() (std::monostate /*unused*/) {}

    template<typename Scene>
    void operator() (Scene& currentScene)
    {
        using enum SceneOperation;
        
        switch (selected)
        {
            case INIT:
                currentScene.init();
                break;

            case DEINIT:
                currentScene.deinit();
                break;
            
            case CLICK:
                currentScene.clicks();
                break;
            
            case HOVER:
                currentScene.intersects(cursor);
                break;
            
            case REDRAW:
                currentScene.redraw();
                break;
            
            case ENABLE:
                currentScene.onSceneLoaded();
                break;
        }
    }
};
