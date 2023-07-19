#include "Event.hpp"
#include "Game.hpp"
#include "Scene.hpp"

/**
 * 
 * Constructor for an event object.
 * 
 */
Event::Event()
{
	// User-invokable events and their method calls
	eventCalls[SDL_QUIT] = [] () { Game::Terminate(); };
	eventCalls[SDL_USEREVENT] = [&, this] () { Scene::Instance().Redraw(); };
	eventCalls[SDL_MOUSEBUTTONDOWN] = [&, this] () { (*this).MouseClick(); };
	eventCalls[SDL_MOUSEMOTION] = [&, this] () { (*this).MouseMotion(gameEvent); };

    // Testing Purposes
    eventCalls[SDL_KEYDOWN] = [] () { Game::Terminate(); };
}

/**
 * 
 * Getter function for the game event object.
 * @return : the address of the game event
 * 
 */
SDL_Event* Event::_IncomingGameEvent()
{
	return &gameEvent;
}

/**
 * 
 * Validator for the incoming events. It's considered valid if we had registered it.
 * @return : either true or false
 * 
 */
bool Event::IsValidEvent() const
{
	if(eventCalls.contains(gameEvent.type))
	{
		return true;
	}

	return false;
}

/**
 * 
 * Call the appropriate method based on the event.
 * 
 */
void Event::ExecuteHandler()
{	
	eventCalls[gameEvent.type]();
}

/**
 * 
 * Handling mouse click events.
 * 
 */
void Event::MouseClick()
{
    if(Scene::Instance().ClickOnCurrentHitId())
    {
        Scene::Instance().Invalidate();
    }
}

/**
 * 
 * Handling mouse movement events.
 * 
 */
void Event::MouseMotion(SDL_Event args)
{
	SDL_Point mousePos = { args.motion.x, args.motion.y };

    if(Scene::Instance().CheckForHit(mousePos))
    {
        Scene::Instance().Invalidate();
    }
}