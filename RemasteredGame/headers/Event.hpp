#ifndef EVENT_HPP
#define EVENT_HPP

#include <functional>
#include <unordered_map>
#include "SDL.h"

using std::unordered_map;
using std::function;

class Event
{
private:
	// Holding object for incoming events
	SDL_Event gameEvent;

	// Outgoing method calls based on SDL events
	unordered_map<unsigned int, function<void()>> eventCalls;

public:
	Event();

	SDL_Event* _IncomingGameEvent();

	bool IsValidEvent() const;

	void ExecuteHandler();

	void MouseClick();

	void MouseMotion(SDL_Event args);
};

#endif