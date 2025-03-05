#pragma once
#include "SDL3/SDL.h"
#include <cstdint>
#include <variant>

enum class SceneOperation : std::uint8_t
{
    Init,
    Deinit,
    Click,
    Hover,
    Redraw,
    Enable
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

    IScene(const SceneOperation oper, const SDL_FPoint pos)
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
            case Init:
                currentScene.init();
                break;

            case Deinit:
                currentScene.deinit();
                break;
            
            case Click:
                currentScene.clicks();
                break;
            
            case Hover:
                currentScene.intersects(cursor);
                break;
            
            case Redraw:
                currentScene.redraw();
                break;
            
            case Enable:
                currentScene.onSceneLoaded();
                break;
        }
    }
};
