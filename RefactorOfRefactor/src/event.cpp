#include "event.hpp"

#include "SDL3/SDL_events.h"
#include "debug.hpp"
#include "defer.hpp"
#include <functional>
#include <unordered_map>
#include <memory>
#include "fontmanager.hpp"
#include "utility.hpp"
#include "scene.hpp"
#include "globals.hpp"

using enum Utility::CustomEvents;
using Globals::GameProperties;

static SDL_Event presentEvent;
static bool continueProcessingEvents = true;
static std::unique_ptr<IScene> currentScene;
static std::unordered_map<uint32_t, std::function<void()>> eventCalls;

static bool isValidEvent()
{
    return eventCalls.contains(presentEvent.type) ? true : false;
}

static void executeCallback()
{
    const auto callback = static_cast<Invokable*>(presentEvent.user.data1);
    ASSERT(callback != nullptr, "Failed to retrieve callback on UI thread");
    DEFER(Utility::free<Invokable>, callback);

    (*callback).execute();
}

static void changeSceneTo()
{
    const auto newScene = static_cast<IScene*>(presentEvent.user.data1);
    currentScene.reset(newScene);
}

static void subscribeToEvents()
{
    eventCalls.emplace(SDL_EVENT_QUIT, []() { continueProcessingEvents = false; (*currentScene).deinit(); });
    eventCalls.emplace(SDL_EVENT_KEY_DOWN, []() {});
    eventCalls.emplace(SDL_EVENT_MOUSE_BUTTON_DOWN, []() { (*currentScene).clicks(); });
    eventCalls.emplace(SDL_EVENT_MOUSE_MOTION, []() { (*currentScene).intersects({ presentEvent.motion.x, presentEvent.motion.y }); });
    eventCalls.emplace(EVENT_INVALIDATE, []() { (*currentScene).redraw(); });
    eventCalls.emplace(EVENT_ENABLE_SCENE, []() { (*currentScene).enable(); });
    eventCalls.emplace(EVENT_INVOKE_ON_UI_THREAD, executeCallback);
    eventCalls.emplace(EVENT_CHANGE_SCENE, changeSceneTo);
}

void Event::processIncomingEvents()
{
    const auto detectedEvents = SDL_WaitEvent(&presentEvent);
    ASSERT(detectedEvents, "Failed to load event from queue ({})", SDL_GetError());

    if (isValidEvent())
    {
        // if (presentEvent.type != 1024)
        // {
        //     LOG("Valid event ({})", presentEvent.type);
        // }
        
        eventCalls[presentEvent.type]();
    }

    if (continueProcessingEvents)
    {
        __TAILREC__
        return processIncomingEvents();
    }
}

void Event::applyDefaultSettings()
{
    const GameProperties properties = Globals::Properties.value();
    SDL_SetWindowSize(properties.Window, properties.ScreenWidth, properties.ScreenHeight);
    SDL_SetWindowFullscreen(properties.Window, true);

    subscribeToEvents();

    FontManager::init();

    currentScene = std::make_unique<MainMenuScene>();
}
