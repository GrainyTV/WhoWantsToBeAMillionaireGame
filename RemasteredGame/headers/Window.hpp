#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <stdexcept>
#include <memory>
#include <functional>
#include <unordered_map>
#include "SDL.h"
#include "WindowProperties.hpp"
#include "Event.hpp"

using std::runtime_error;
using std::unique_ptr;
using std::unordered_map;
using std::function;

class Window
{
private:
	// Signal wether the game is still running
	static bool running;
	
	// Handler for the incoming events
	SDL_Event gameEvent;

	// Handler for the outgoing events
	Event invokedEvent;

	// Outgoing method calls based on events
	unordered_map<unsigned int, function<void()>> eventCalls;
	
	// Automatically freed window properties object
	unique_ptr<WindowProperties> properties;

public:
	Window();

	void Launch();

	void Terminate();
};

#endif