#pragma once
#include "scenes.hpp"
#include <functional>
#include <unordered_map>
#include <variant>

enum CustomEvents
{
    EVENT_INVALIDATE = SDL_EVENT_USER,
    EVENT_ENABLE_SCENE,
    EVENT_INVOKE_ON_UI_THREAD,
};

using ExclusiveScenes = std::variant<MainMenuScene, InGameScene>;

class Event
{
private:
    SDL_Event presentEvent;
    bool continueProcessingEvents;
    std::optional<ExclusiveScenes> currentScene;
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
};
