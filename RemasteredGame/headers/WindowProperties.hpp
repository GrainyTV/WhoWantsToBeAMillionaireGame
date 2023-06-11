#ifndef WINDOWPROPERTIES_HPP
#define WINDOWPROPERTIES_HPP

#include <stdexcept>
#include "SDL.h"

using std::runtime_error;

class WindowProperties
{
private:
	static int SCREEN_WIDTH;
	static int SCREEN_HEIGHT;
	SDL_Window* window;
	SDL_Renderer* renderer;

public:
	WindowProperties();
};

#endif