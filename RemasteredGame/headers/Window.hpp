#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <stdexcept>
#include "SDL.h"

using std::runtime_error;

class Window
{
private:
	static int SCREEN_WIDTH;
	static int SCREEN_HEIGHT;
	SDL_Window* window;
	SDL_Renderer* renderer;

public:
	Window();

	~Window();

	SDL_Renderer* _Renderer() const;
};

#endif