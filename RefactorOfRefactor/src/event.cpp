#include "event.hpp"

#include "SDL3/SDL_events.h"
#include "debug.hpp"
#include "defer.hpp"
#include <functional>
#include <unordered_map>
#include "utility.hpp"
#include "iscene.hpp"
#include "mainmenuscene.hpp"
#include "ingamescene.hpp"
#include "globals.hpp"

using ExclusiveScenes = std::variant<std::monostate, MainMenuScene, InGameScene>;
using enum Utility::CustomEvents;
using Globals::GameProperties;

static SDL_Event presentEvent;
static bool continueProcessingEvents = true;
static ExclusiveScenes currentScene;
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
    const size_t newSceneId = presentEvent.user.code;

    if (newSceneId != currentScene.index())
    {
        std::visit(IScene(SceneOperation::Deinitialize), currentScene);

        switch (newSceneId)
        {
            case 1:
                currentScene.emplace<MainMenuScene>();
                break;

            case 2:
                currentScene.emplace<InGameScene>();
                break;

            default:
                ASSERT(false, "Tried to access an unregistered scene with index ()", newSceneId);
                break;
        }

        Utility::invalidate();
    }
}

static void subscribeToEvents()
{
    eventCalls.emplace(SDL_EVENT_QUIT, []() { continueProcessingEvents = false; std::visit(IScene(SceneOperation::Deinitialize), currentScene); });
    eventCalls.emplace(SDL_EVENT_KEY_DOWN, []() {});
    eventCalls.emplace(SDL_EVENT_MOUSE_BUTTON_DOWN, []() { std::visit(IScene(SceneOperation::Clicks), currentScene); });
    eventCalls.emplace(SDL_EVENT_MOUSE_MOTION, []() { std::visit(IScene(SceneOperation::Intersects, presentEvent.motion.x, presentEvent.motion.y), currentScene); });
    eventCalls.emplace(EVENT_INVALIDATE, []() { std::visit(IScene(SceneOperation::Redraw), currentScene); });
    eventCalls.emplace(EVENT_ENABLE_SCENE, []() { std::visit(IScene(SceneOperation::Enable), currentScene); });
    eventCalls.emplace(EVENT_INVOKE_ON_UI_THREAD, executeCallback);
    eventCalls.emplace(EVENT_CHANGE_SCENE, changeSceneTo);
}

void Event::processIncomingEvents()
{
    const auto detectedEvents = SDL_WaitEvent(&presentEvent);
    ASSERT(detectedEvents, "Failed to load event from queue ({})", SDL_GetError());

    if (isValidEvent())
    {
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

    currentScene.emplace<MainMenuScene>();
}
