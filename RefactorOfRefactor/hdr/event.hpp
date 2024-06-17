#pragma once
#include <functional>
#include <unordered_map>
#include <variant>
// #include "SDL3/SDL.h"
#include "scenes.hpp"

class Event
{
private:
    SDL_Event presentEvent;
    bool continueProcessingEvents;
    std::variant<MainMenuScene, InGameScene> currentScene;
    std::unordered_map<uint32_t, std::function<void()>> eventCalls;

    void terminate();

    void unhideWindowAtStart();

    bool isValidEvent() const;

public:
    Event();

    void processIncomingEvents();

    void invalidate();

    /*template<typename Func, typename Obj, typename... Args>
    void invoke(Func&& func, Obj&& obj, Args&&... args)
    {
        const auto delegate = [&]() {
            (*obj.*func)(std::forward<Args>(args)...);
        };

        delegate();
    }*/

    /*template<typename Func, typename... Args>
    void invoke(Func function, Args&&... arguments)
    {
        std::invoke(function, std::forward<Args>(arguments)...);
    }

    template<typename Func, typename Obj, typename... Args>
    void invoke(Func function, Obj& object, Args&&... arguments)
    {
        std::invoke(function, object, std::forward<Args>(arguments)...);
    }*/

    template<typename Func, typename... Args>
    void invoke(Func&& function, Args&&... arguments)
    {
        std::invoke(std::forward<Func>(function), std::forward<Args>(arguments)...);
    }
};
