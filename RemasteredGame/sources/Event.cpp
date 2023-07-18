#include "Event.hpp"
#include "Game.hpp"
#include "Scene.hpp"

/**
 * 
 * Constructor with a copy of our scene object.
 * @param scene : a pointer to our previously created scene
 * 
 */
Event::Event() //(Scene* sceneAddr) : scenePtr(sceneAddr)
{
	invalidator.type = SDL_RegisterEvents(1);

	// User-invokable events and their method calls
	eventCalls[SDL_QUIT] = [] () { Game::Terminate(); };
	eventCalls[SDL_USEREVENT] = [&, this] () { Scene::Instance().Redraw(); /*(*scenePtr).Redraw();*/ };
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
 * Method to push an invalidate call onto the event stack.
 * 
 */
void Event::Invalidate()
{
	SDL_PushEvent(&invalidator);
}

/**
 * 
 * Handling mouse click events.
 * 
 */
void Event::MouseClick()
{
    if(Scene::Instance().ClickOnCurrentHitId()) //((*scenePtr).ClickOnCurrentHitId())
    {
        (*this).Invalidate();
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

    if(Scene::Instance().CheckForHit(mousePos)) //((*scenePtr).CheckForHit(mousePos))
    {
        (*this).Invalidate();
    }
}