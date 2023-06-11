#include "Window.hpp"

bool Window::running = true;

Window::Window()
{
	this->properties = unique_ptr<WindowProperties>(new WindowProperties());
	this->invokedEvent = Event();
	this->eventCalls[SDL_QUIT] = [this]() { this->Terminate(); };
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