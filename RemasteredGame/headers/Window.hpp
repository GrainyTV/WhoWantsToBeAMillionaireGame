#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <stdexcept>
#include "SDL.h"

using std::runtime_error;

class Window
{
private:
	// Width of the window
	static int SCREEN_WIDTH;

	// Height of the window
	static int SCREEN_HEIGHT;

	// Our window object
	SDL_Window* window;

	// Our renderer object
	SDL_Renderer* renderer;

public:
	Window();

	~Window();

	SDL_Renderer* _Renderer() const;
};

#endif