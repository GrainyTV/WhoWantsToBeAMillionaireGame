#ifndef EVENT_HPP
#define EVENT_HPP

#include <functional>
#include <unordered_map>
#include <memory>
#include "SDL.h"
#include "Scene.hpp"

using std::unordered_map;
using std::function;
using std::bind;
using std::shared_ptr;

class Event
{
private:
	SDL_Event gameEvent;

	Scene* scenePtr;

	// Outgoing method calls based on events
	unordered_map<unsigned int, function<void()>> eventCalls;

public:
	Event();

	Event(Scene& scene);

	SDL_Event* _IncomingGameEvent();

	bool IsValidEvent() const;

	void ExecuteHandler();

	void MouseClick();

	void MouseMotion();
};

#endif