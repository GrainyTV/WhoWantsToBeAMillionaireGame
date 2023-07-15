#ifndef GAMEVIEW_HPP
#define GAMEVIEW_HPP

#include "Window.hpp"
#include "Event.hpp"
#include "Scene.hpp"

class GameView
{
private:
	// The application's window and its properties
	Window window;	

	// The actual contents of the window
	Scene scene;

	// An event handler for the interactivity
	Event event;

public:
	GameView();

	void EnterGameLoop();
};

#endif