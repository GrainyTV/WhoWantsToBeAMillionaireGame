#include "Window.hpp"

int Window::SCREEN_WIDTH = 960;

int Window::SCREEN_HEIGHT = 540;

Window::Window()
{
	if(SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		throw runtime_error(SDL_GetError());
	}

	window = SDL_CreateWindow("Legyen on is milliomos!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL);

	if(window == NULL)
	{
		throw runtime_error(SDL_GetError());
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	if(renderer == NULL)
	{
		throw runtime_error(SDL_GetError());
	}
}

Window::~Window()
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

SDL_Renderer* Window::_Renderer() const
{
	return renderer;
}