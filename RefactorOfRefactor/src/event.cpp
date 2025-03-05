#include "event.hpp"
#include "debug.hpp"
#include "defer.hpp"
#include "fontmanager.hpp"
#include "globals.hpp"
#include "ingamescene.hpp"
#include "iscene.hpp"
#include "mainmenuscene.hpp"
#include "opengl.hpp"
#include "utility.hpp"
#include <functional>
#include <unordered_map>

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
            auto* const callback = static_cast<Invokable*>(presentEvent.user.data1);
            assert(callback != nullptr, "Failed to retrieve callback on UI thread");
            DEFER(Utility::free<Invokable>, callback);

            (*callback).execute();
        }

        template<typename Scene>
        void changeSceneTo()
        {
            currentScene.emplace<Scene>();
            std::visit(IScene(Init), currentScene);
        }

        std::unordered_map<uint32_t, std::function<void()>> eventCalls =
        {
            { SDL_EVENT_QUIT, []() { continueRunning = false; std::visit(IScene(Deinit), currentScene); }},
            { SDL_EVENT_MOUSE_BUTTON_DOWN, []() { std::visit(IScene(Click), currentScene); }},
            { SDL_EVENT_MOUSE_MOTION, []() { std::visit(IScene(Hover, { presentEvent.motion.x, presentEvent.motion.y }), currentScene); }},
            { EVENT_INVALIDATE, []() { std::visit(IScene(Redraw), currentScene); }},
            { EVENT_ENABLE_SCENE, []() { std::visit(IScene(Enable), currentScene); }},
            { EVENT_INVOKE_ON_UI_THREAD, executeCallback },
            { EVENT_CHANGE_SCENE_MAINMENU, changeSceneTo<MainMenuScene> },
            { EVENT_CHANGE_SCENE_INGAME, changeSceneTo<InGameScene> },
        };
    }

    void startRunningOnMainUiThread()
    {
        const auto properties = Globals::Properties.value();
        SDL_SetWindowSize(properties.Window, properties.ScreenWidth, properties.ScreenHeight);
        SDL_SetWindowFullscreen(properties.Window, true);

        OpenGL::compileShaders();
        FontManager::init();
        changeSceneTo<MainMenuScene>();

        while (continueRunning)
        {
            const bool detectedEvent = SDL_WaitEvent(&presentEvent);
            assert(detectedEvent, "Failed to load event from queue ({})", SDL_GetError());

            if (eventCalls.contains(presentEvent.type))
            {
                eventCalls[presentEvent.type]();
            }
        }
    }
}
