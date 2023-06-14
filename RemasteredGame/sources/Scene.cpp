#include "Scene.hpp"

random_device Scene::randomGenerator = random_device();

Scene::Scene()
{
}

Scene::Scene(SDL_Renderer* renderer)
{
	sceneRenderer = renderer;
	invalidator.type = SDL_RegisterEvents(1);
}

void Scene::Redraw()
{
	if(SDL_SetRenderDrawColor(sceneRenderer, 0, 0, 0, 255) != 0)
	{
		throw runtime_error(SDL_GetError());
	}

	if(SDL_RenderClear(sceneRenderer) != 0)
	{
		throw runtime_error(SDL_GetError());
	}

	printf("%s\n", "Redraw Invoked!");
	//SDL_RenderCopy(sceneRenderer, textures[currentState][currentTexture], NULL, NULL);

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