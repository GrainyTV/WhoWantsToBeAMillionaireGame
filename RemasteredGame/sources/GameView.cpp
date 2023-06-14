#include "GameView.hpp"
#include "Game.hpp"

GameView::GameView()
{
	scene = Scene(window._Renderer());
	event = Event(scene);
}

void GameView::EnterGameLoop()
{
	while(Game::IsRunning())
	{
		while(SDL_PollEvent(event._IncomingGameEvent()))
		{
			if(event.IsValidEvent())
			{
				event.ExecuteHandler();
			}
		}
	}
}