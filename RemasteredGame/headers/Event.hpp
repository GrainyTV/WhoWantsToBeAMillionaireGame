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

	// Pointer to the original scene object
	Scene* scenePtr;

	// Outgoing method calls based on events
	unordered_map<unsigned int, function<void()>> eventCalls;

	// Our buttons, categorized by game state
	unordered_map<GameState, deque<SDL_Rect>> buttonHitboxes;

	// Our methods that execute on button press, categorized by game state
	unordered_map<GameState, deque<function<void()>>> buttonHitboxesClicked;

public:
	Event(Scene* scene);

	SDL_Event* _IncomingGameEvent();

	bool IsValidEvent() const;

	void ExecuteHandler();

	void MouseClick();

	void MouseMotion(SDL_Event args);

	void FillMouseHitboxes();

	static bool Hit(const SDL_Rect& r1, const SDL_Rect& r2);
};

#endif