#include "event.hpp"
#include "assert.hpp"
#include "defer.hpp"
#include "invokable.hpp"

Event::Event()
    : continueProcessingEvents(true)
    , eventCalls({
          { SDL_EVENT_QUIT, [&]() { continueProcessingEvents = false; } },
          { SDL_EVENT_KEY_DOWN, [&]() { invalidate(); } },
          { SDL_EVENT_MOUSE_BUTTON_DOWN, [&]() { requestEvent({ .type = SDL_EVENT_QUIT }); } },
          { SDL_EVENT_WINDOW_SHOWN, [&]() { unhideWindowAtStart(); } },
          { EVENT_INVALIDATE, [&]() { std::visit(SceneRedrawer{}, *currentScene); } },
          { EVENT_INVOKE_ON_UI_THREAD, [&]() { executeCallback(); } },
      })
{}

void Event::applyDefaultScene()
{
    // ==================================================== //
    // Constructs a Scene collection object in-place        //
    // A variant ALWAYS holds its first type as default     //
    // MainMenuScene should be the first template parameter //
    // ==================================================== //

    currentScene.emplace();
}

void Event::processIncomingEvents()
{
    const auto detectEvents = SDL_WaitEvent(&presentEvent);
    ASSERT(detectEvents == true, "Failed to load event from queue ({})", SDL_GetError());

    if (isValidEvent())
    {
        eventCalls[presentEvent.type]();
    }

    if (continueProcessingEvents)
    {
#ifdef __clang__
        [[clang::musttail]]
#endif
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
    DEFER([&callback]() { delete callback; });

    (*callback).execute();
}