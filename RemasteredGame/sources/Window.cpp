#include "Window.hpp"

int Window::SCREEN_WIDTH = 960;

int Window::SCREEN_HEIGHT = 540;

SDL_Window* Window::window = nullptr;

SDL_Renderer* Window::renderer = nullptr;

/**
 * 
 * Constructor initializes subsystems, the window and the renderer.
 * 
 */
Window::Window()
{
	if(SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		throw runtime_error(SDL_GetError());
	}

	// Turn on anti-aliasing for surfaces
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 8);

	window = SDL_CreateWindow("Legyen on is milliomos!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN);

	if(window == NULL)
	{
		throw runtime_error(SDL_GetError());
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	
	if(renderer == NULL)
	{
		throw runtime_error(SDL_GetError());
	}

	// Turn on anti-aliasing for textures
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
}

/**
 * 
 * Destructor deallocates reserved resources automatically.
 * 
 */
Window::~Window()
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

/**
 * 
 * Getter function for the renderer object.
 * 
 */
SDL_Renderer* Window::_Renderer()
{
	return renderer;
}

/**
 * 
 * Getter function for the window object.
 * 
 */
SDL_Window* Window::_Window()
{
	return window;
}