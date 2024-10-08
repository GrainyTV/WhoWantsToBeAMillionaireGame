#pragma once
#include "SDL3/SDL.h"
#include <variant>

enum class SceneOperation
{
    Deinitialize,
    Redraw,
    Intersects,
    Clicks,
    Enable,
};

class IScene
{
private:
    SceneOperation operation;
    SDL_FPoint location;

public:
    IScene(SceneOperation op)
        : operation(op)
    {}

    IScene(SceneOperation op, float x, float y)
        : operation(op)
        , location({ .x = x, .y = y })
    {}

    void operator()(std::monostate) const {}

    template<typename Scene>
    void operator()(Scene&& scene) const
    {
        switch (operation)
        {
            case SceneOperation::Deinitialize:
                scene.deinit();
                break;

            case SceneOperation::Redraw:
                scene.redraw();
                break;

            case SceneOperation::Intersects:
                scene.intersects(location);
                break;

            case SceneOperation::Clicks:
                scene.clicks();
                break;

            case SceneOperation::Enable:
                scene.enable();
                break;
        }
    }
};
