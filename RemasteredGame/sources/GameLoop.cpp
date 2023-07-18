#include "SDL_image.h"
#include "SDL_ttf.h"
#include "GameLoop.hpp"
#include "Game.hpp"

/**
 * 
 * Constructor immediately asks for a redraw at the beginning.
 * 
 */
GameLoop::GameLoop()
{
	if(IMG_Init(IMG_INIT_PNG) == 0)
	{
		throw runtime_error("Failed to load Image library.");
	}

	if(TTF_Init() != 0)
	{
		throw runtime_error(TTF_GetError());
	}

	event.Invalidate();
}

/**
 * 
 * GameLoop destructor closes Image and TTF library.
 * 
 */
GameLoop::~GameLoop()
{
	TTF_Quit();
	IMG_Quit();
}

/**
 * 
 * The game loop of our application. It runs constantly on the main thread until termination is called.
 * 
 */
void GameLoop::EnterGameLoop()
{
	while(Game::IsRunning())
	{
		SDL_PollEvent(event._IncomingGameEvent());

		if(event.IsValidEvent())
		{
			event.ExecuteHandler();
		}
	}
}