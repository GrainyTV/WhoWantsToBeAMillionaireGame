#ifndef GAMELOOP_HPP
#define GAMELOOP_HPP

#include "Window.hpp"
#include "Event.hpp"

class GameLoop
{
private:
	// The application's window and its properties
	Window window;

	// An event handler for the interactivity
	Event event;

public:
	GameLoop();

	~GameLoop();

	void EnterGameLoop();
};

#endif