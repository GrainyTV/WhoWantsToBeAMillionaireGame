#ifndef SCENE_HPP
#define SCENE_HPP

#include <stdexcept>
#include <random>
#include "SDL.h"

using std::runtime_error;
using std::random_device;
using std::mt19937;
using std::uniform_int_distribution;

enum GameState { MainMenu, Options, InGame };

class Scene
{
private:
	// Copy of the renderer object
	SDL_Renderer* sceneRenderer;

	// Event to call when screen needs redraw
	SDL_Event invalidator;

	// Expensive non-deterministic seed for random numbers
	static random_device randomGenerator;

public:
	Scene();

	Scene(SDL_Renderer* renderer);

	void Redraw();

	void Invalidate();

	static int Random(int lowerBound, int upperBound);
};

#endif