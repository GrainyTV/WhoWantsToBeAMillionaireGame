#ifndef EVENT_HPP
#define EVENT_HPP

#include <functional>
#include <unordered_map>
#include <memory>
#include <deque>
#include "SDL.h"
#include "Scene.hpp"

using std::unordered_map;
using std::function;
using std::bind;
using std::shared_ptr;
using std::placeholders::_1;
using std::deque;

class Event
{
private:
	// Holding object for incoming events
	SDL_Event gameEvent;

	// Event to call when screen needs redraw
	SDL_Event invalidator;

	// Pointer to the original scene object
	Scene* scenePtr;

	// Outgoing method calls based on events
	unordered_map<unsigned int, function<void()>> eventCalls;

public:
	Event(Scene* scene);

	SDL_Event* _IncomingGameEvent();

	bool IsValidEvent() const;

	void ExecuteHandler();

	void Invalidate();

	void MouseClick();

	void MouseMotion(SDL_Event args);
};

#endif