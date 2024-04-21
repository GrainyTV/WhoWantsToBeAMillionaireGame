#include "event.hpp"
#include "assert.hpp"

Event::Event()
    : firstDraw(true)
    , continueProcessingEvents(true)
    , currentScene(MainMenuScene())
{
    eventCalls.insert({ SDL_EVENT_QUIT, [&]() { continueProcessingEvents = false; }});
    eventCalls.insert({ SDL_EVENT_USER, [&]() { std::visit(SceneResolver{}, currentScene); }});
    eventCalls.insert({ SDL_EVENT_KEY_DOWN, [this]() { (*this).invalidate(); }});
    eventCalls.insert({ SDL_EVENT_MOUSE_BUTTON_DOWN, [this]() { (*this).terminate(); }});
    eventCalls.insert({ SDL_EVENT_WINDOW_SHOWN, [this]() { (*this).unhideWindowAtStart(); }});
}

void Event::processIncomingEvents()
{
    const auto detectEvents = SDL_WaitEvent(&event);
    ASSERT(detectEvents == SDL_TRUE, "Failed to load event from queue with {}", SDL_GetError());

    if (isValidEvent())
    {
        eventCalls[event.type]();
    }
    
    if (continueProcessingEvents)
    {
        [[clang::musttail]]
        return processIncomingEvents();
    }
}

bool Event::isValidEvent() const
{
    return eventCalls.contains(event.type) ? true : false;
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
    if (firstDraw)
    {
        invalidate();
        firstDraw = false;
    }
}