#ifndef GAMEVIEW_HPP
#define GAMEVIEW_HPP

#include "SDL.h"
#include "Window.hpp"
#include "Event.hpp"
#include "Scene.hpp"

class GameView
{
private:
	Window window;	

	Scene scene;

	Event event;

public:
	GameView();

	void EnterGameLoop();
};

#endif