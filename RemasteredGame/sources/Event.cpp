#include "Event.hpp"
#include "Game.hpp"

Event::Event()
{
}

Event::Event(Scene& scene)
{
	scenePtr = &scene;

	// User-invokable events and their method calls
	eventCalls[SDL_QUIT] = Game::Terminate;
	eventCalls[SDL_USEREVENT] = bind(&Scene::Redraw, scene);;
	eventCalls[SDL_MOUSEBUTTONDOWN] = bind(&Event::MouseClick, (*this));;
	eventCalls[SDL_MOUSEMOTION] = bind(&Event::MouseMotion, (*this));;
}

SDL_Event* Event::_IncomingGameEvent()
{
	return &gameEvent;
}

bool Event::IsValidEvent() const
{
	if(eventCalls.contains(gameEvent.type))
	{
		return true;
	}

	return false;
}

void Event::ExecuteHandler()
{
	eventCalls[gameEvent.type]();
}

void Event::MouseClick()
{
	(*scenePtr).Invalidate();
}

void Event::MouseMotion()
{
	(*scenePtr).Invalidate();
}