#pragma once
#include <unordered_map>
#include <functional>
#include <variant>
#include "SDL3/SDL.h"
#include "scene.hpp"

class Event
{
private:
    bool firstDraw;
    SDL_Event event;
    bool continueProcessingEvents;
    std::variant<MainMenuScene, InGameScene> currentScene;
    std::unordered_map<uint32_t, std::function<void()>> eventCalls;

public:
    Event();

    void processIncomingEvents();

    bool isValidEvent() const;

    void invalidate();

    void terminate();

    void unhideWindowAtStart();
};