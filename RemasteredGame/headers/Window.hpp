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
	static SDL_Window* window;

	// Our renderer object
	static SDL_Renderer* renderer;

public:
	Window();

	~Window();

	static SDL_Renderer* _Renderer();

	static SDL_Window* _Window();

	static int _Width();

	static int _Height();
};

#endif