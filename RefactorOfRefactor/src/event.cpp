#include "event.hpp"
#include "debug.hpp"
#include "defer.hpp"
#include <functional>
#include <unordered_map>
#include "fontmanager.hpp"
#include "ingamescene.hpp"
#include "mainmenuscene.hpp"
#include "utility.hpp"
#include "globals.hpp"
#include "iscene.hpp"

namespace Event
{
    namespace
    {
        using enum Utility::CustomEvents;
        using enum SceneOperation;
        using ExclusiveScene = std::variant<std::monostate, MainMenuScene, InGameScene>;

        SDL_Event presentEvent;
        ExclusiveScene currentScene;
        bool continueRunning = true;

        void executeCallback()
        {
            const auto callback = static_cast<Invokable*>(presentEvent.user.data1);
            ASSERT(callback != nullptr, "Failed to retrieve callback on UI thread");
            DEFER(Utility::free<Invokable>, callback);

            (*callback).execute();
        }

        // void changeSceneTo()
        // {
        //     const auto newScene = static_cast<IScene*>(presentEvent.user.data1);
        //     currentScene.reset(newScene);
        // }

        // void changeSceneToMainMenu()
        // {
        //     currentScene.emplace<MainMenuScene>();
        // }

        void changeSceneToInGame()
        {
            currentScene.emplace<InGameScene>();
        }

        // void changeSceneToSettings()
        // {
        //     currentScene.emplace<MainMenuScene>();
        // }

        std::unordered_map<uint32_t, std::function<void()>> eventCalls =
        {
            { SDL_EVENT_QUIT, []() { continueRunning = false; std::visit(IScene(Deinitialize), currentScene); }},
            { SDL_EVENT_MOUSE_BUTTON_DOWN, []() { std::visit(IScene(Click), currentScene); }},
            { SDL_EVENT_MOUSE_MOTION, []() { std::visit(IScene(Hover, { presentEvent.motion.x, presentEvent.motion.y }), currentScene); }},
            { EVENT_INVALIDATE, []() { std::visit(IScene(Redraw), currentScene); }},
            { EVENT_ENABLE_SCENE, []() { std::visit(IScene(Enable), currentScene); }},
            { EVENT_INVOKE_ON_UI_THREAD, executeCallback },
            { EVENT_CHANGE_SCENE, changeSceneToInGame }
        };
    }

    void processIncomingEvents()
    {
        while (continueRunning)
        {
            const auto detectedEvent = SDL_WaitEvent(&presentEvent);
            ASSERT(detectedEvent, "Failed to load event from queue ({})", SDL_GetError());

            if (eventCalls.contains(presentEvent.type))
            {
                eventCalls[presentEvent.type]();
            }
        }
    }

    void applyDefaultSettings()
    {
        const auto properties = Globals::Properties.value();
        SDL_SetWindowSize(properties.Window, properties.ScreenWidth, properties.ScreenHeight);
        SDL_SetWindowFullscreen(properties.Window, true);

        FontManager::init();

        currentScene.emplace<MainMenuScene>();
    }
}


