#pragma once
#include "SDL3/SDL.h"
#include <variant>

enum class SceneOperation
{
    Deinitialize,
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
    IScene(const SceneOperation op)
        : selected(op)
    {}

    IScene(const SceneOperation op, const SDL_FPoint pos)
        : selected(op)
        , cursor(pos)
    {}

    void operator() (std::monostate) {}

    template<typename Scene>
    void operator() (Scene& currentScene)
    {
        using enum SceneOperation;
        
        switch (selected)
        {
            case Deinitialize:
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
                currentScene.enable();
                break;
        }
    }
};
