#include "event.hpp"
#include "assert.hpp"

Event::Event()
    : continueProcessingEvents(true)
    , eventCalls({
          { SDL_EVENT_QUIT, [&]() { continueProcessingEvents = false; } },
          { SDL_EVENT_USER, [&]() { std::visit(SceneRedrawer{}, currentScene); } },
          { SDL_EVENT_KEY_DOWN, [this]() { (*this).invalidate(); } },
          { SDL_EVENT_MOUSE_BUTTON_DOWN, [this]() { (*this).terminate(); } },
          { SDL_EVENT_WINDOW_SHOWN, [this]() { (*this).unhideWindowAtStart(); } },
      })
{}

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

void Event::invalidate()
{
    SDL_Event invalidateEvent;
    invalidateEvent.type = SDL_EVENT_USER;
    SDL_PushEvent(&invalidateEvent);
}

void Event::terminate()
{
    SDL_Event terminationEvent;
    terminationEvent.type = SDL_EVENT_QUIT;
    SDL_PushEvent(&terminationEvent);
}

void Event::unhideWindowAtStart()
{
    invalidate();
    eventCalls.erase(SDL_EVENT_WINDOW_SHOWN);
}