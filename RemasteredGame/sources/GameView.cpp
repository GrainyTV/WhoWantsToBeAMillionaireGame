#include "GameView.hpp"
#include "Game.hpp"

/**
 * 
 * Constructor sets the default scene and event objects.
 * 
 */
GameView::GameView() : event(&scene)
{
	event.Invalidate();
}

/**
 * 
 * The game loop of our application. It runs constantly on the main thread until termination is called.
 * 
 */
void GameView::EnterGameLoop()
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