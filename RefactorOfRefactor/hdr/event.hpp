#pragma once
#include "ingamescene.hpp"
#include "mainmenuscene.hpp"
#include "sceneinterface.hpp"
#include <functional>
#include <unordered_map>
#include <variant>

enum CustomEvents
{
    EVENT_INVALIDATE = SDL_EVENT_USER,
    EVENT_ENABLE_SCENE,
    EVENT_INVOKE_ON_UI_THREAD,
};

using ExclusiveScenes = std::variant<DefaultScene, MainMenuScene, InGameScene>;

class Event
{
private:
    SDL_Event presentEvent;
    bool continueProcessingEvents;
    ExclusiveScenes currentScene;
    std::unordered_map<uint32_t, std::function<void()>> eventCalls;

private:
    void unhideWindowAtStart();

    bool isValidEvent() const;

    void executeCallback() const;

public:
    Event();

    void applyDefaultScene();

    void processIncomingEvents();

    void invalidate();

    void requestEvent(SDL_Event&& newEvent);

    template<typename NewScene>
    void changeSceneTo()
    {
        std::visit(SceneInterface{SceneOperation::Deinitialize}, currentScene);
        currentScene.emplace<NewScene>();
        invalidate();
    }
};
