#include "Scene.hpp"

random_device Scene::randomGenerator = random_device();

Scene::Scene()
{
}

Scene::Scene(const WindowProperties& properties)
{
	sceneRenderer = properties._Renderer();
	invalidator.type = SDL_RegisterEvents(1);
}

void Scene::Redraw()
{
	int R = Random(0, 255);
	int G = Random(0, 255);
	int B = Random(0, 255);

	printf("(%d %d %d)\n", R, G, B);

	if(SDL_SetRenderDrawColor(sceneRenderer, R, G, B, 255) != 0)
	{
		throw runtime_error(SDL_GetError());
	}

	if(SDL_RenderClear(sceneRenderer) != 0)
	{
		throw runtime_error(SDL_GetError());
	}

	SDL_RenderPresent(sceneRenderer);
}

void Scene::Invalidate()
{
	SDL_PushEvent(&invalidator);
}

int Scene::Random(int lowerBound, int upperBound)
{
	// seeding the engine:
	mt19937 engine{ randomGenerator() }; 
	uniform_int_distribution<int> distribution{ lowerBound, upperBound };

	// get random numbers with:
	return distribution(engine);
}