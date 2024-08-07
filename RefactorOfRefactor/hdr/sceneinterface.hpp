#pragma once
#include "SDL3/SDL.h"

enum class SceneOperation
{
    Deinitialize,
    Redraw,
    Intersects,
    Clicks,
};

class SceneInterface
{
private:
    SceneOperation operation;
    SDL_FPoint location;

public:
    SceneInterface(SceneOperation op)
        : operation(op)
    {}

    SceneInterface(SceneOperation op, float x, float y)
        : operation(op)
        , location({ .x = x, .y = y })
    {}

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
        }
    }
};

struct DefaultScene
{
    void deinit() const
    {}

    void redraw() const
    {}

    void intersects(const SDL_FPoint)
    {}

    void clicks()
    {}
};
