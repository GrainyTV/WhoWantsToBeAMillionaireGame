#include "Window.hpp"

bool Window::running = true;

Window::Window()
{
	properties = unique_ptr<WindowProperties>(new WindowProperties());
	invokedEvent = Event();
	scene = Scene(*properties.get());
	
	// User-invokable events and their method calls
	eventCalls[SDL_QUIT] = [this]() { Terminate(); };
	eventCalls[SDL_USEREVENT] = [this]() { scene.Redraw(); };
	eventCalls[SDL_KEYDOWN] = [this]() { scene.Invalidate(); };
}

void Window::Launch()
{
	while(running)
	{
		while(SDL_PollEvent(&gameEvent))
		{
			if(eventCalls.contains(gameEvent.type))
			{
				eventCalls[gameEvent.type]();
			}
		}
	}
}

void Window::Terminate()
{
	running = false;
}