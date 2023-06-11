#include "WindowProperties.hpp"

int WindowProperties::SCREEN_WIDTH = 960;

int WindowProperties::SCREEN_HEIGHT = 540;

WindowProperties::WindowProperties()
{
	if(SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		throw runtime_error(SDL_GetError());
	}

	this->window = SDL_CreateWindow("Legyen on is milliomos!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL);

	if(window == NULL)
	{
		throw runtime_error(SDL_GetError());
	}

	this->renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	if(renderer == NULL)
	{
		throw runtime_error(SDL_GetError());
	}
}