#ifndef WINDOW_HPP
#define WINDOW_HPP

#include "SDL.h"

class Window
{
private:
	static SDL_Window* window;

	static SDL_Renderer* renderer;

public:
	Window()
	{
		window = SDL_CreateWindow("SDL", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1920, 1080, SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN);
		renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
	}

	static SDL_Window* _Window()
	{
		return window;
	}

	static SDL_Renderer* _Renderer()
	{
		return renderer;
	}
};

#endif