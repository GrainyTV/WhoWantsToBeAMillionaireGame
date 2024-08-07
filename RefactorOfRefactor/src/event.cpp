#include "event.hpp"
#include "debug.hpp"
#include "defer.hpp"
#include "extensions.hpp"
#include "invokable.hpp"

Event::Event()
    : continueProcessingEvents(true)
    , eventCalls({
          { SDL_EVENT_QUIT, [&]() { continueProcessingEvents = false; std::visit(SceneInterface{SceneOperation::Deinitialize}, currentScene); } },
          { SDL_EVENT_KEY_DOWN, [&]() {} },
          { SDL_EVENT_MOUSE_BUTTON_DOWN, [&]() { std::visit(SceneInterface{SceneOperation::Clicks}, currentScene); } },
          { SDL_EVENT_MOUSE_MOTION, [&]() { std::visit(SceneInterface{SceneOperation::Intersects, presentEvent.motion.x, presentEvent.motion.y }, currentScene); } },
          { SDL_EVENT_WINDOW_SHOWN, [&]() { unhideWindowAtStart(); } },
          { EVENT_INVALIDATE, [&]() { std::visit(SceneInterface{SceneOperation::Redraw}, currentScene); } },
          { EVENT_ENABLE_SCENE, [&]() { if (MainMenuScene* mms = std::get_if<MainMenuScene>(&currentScene)) { (*mms).enable(); } } },
          { EVENT_INVOKE_ON_UI_THREAD, [&]() { executeCallback(); } },
      })
{}

void Event::applyDefaultScene()
{
    currentScene.emplace<MainMenuScene>();
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

bool Event::isValidEvent() const
{
    return eventCalls.contains(presentEvent.type) ? true : false;
}

void Event::requestEvent(SDL_Event&& newEvent)
{
    SDL_PushEvent(&newEvent);
}

void Event::invalidate()
{
    requestEvent({ .type = EVENT_INVALIDATE });
}

void Event::unhideWindowAtStart()
{
    invalidate();
    eventCalls.erase(SDL_EVENT_WINDOW_SHOWN);
}

void Event::executeCallback() const
{
    const auto callback = static_cast<Invokable*>(presentEvent.user.data1);
    ASSERT(callback != NULL, "Failed to retrieve callback on UI thread");
    DEFER(ext::free<Invokable>, callback);

    (*callback).execute();
}
