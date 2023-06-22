#include "GameView.hpp"
#include "Game.hpp"

/**
 * 
 * Constructor sets the default scene and event objects.
 * 
 */
GameView::GameView() : scene(Window::_Renderer()), event(&scene)
{
	scene.Invalidate();
}

/**
 * 
 * The game loop of our application. It runs continually on the main thread until termination is called.
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