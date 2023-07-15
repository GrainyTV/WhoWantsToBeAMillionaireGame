#include "SDL_image.h"
#include "SDL_ttf.h"
#include "Scene.hpp"
#include "MainMenuScene.hpp"
#include "InGameScene.hpp"

/**
 * 
 * Constructor with the renderer acquired.
 * @param renderer : renderer received from window
 * 
 */
Scene::Scene(SDL_Renderer* renderer)
{
	if(IMG_Init(IMG_INIT_PNG) == 0)
	{
		throw runtime_error("Failed to load Image library.");
	}

	if(TTF_Init() != 0)
	{
		throw runtime_error(TTF_GetError());
	}

	rendererPtr = renderer;
	gameState = Enumeration({ "MainMenu", "InGame" });
	currentState = gameState["MainMenu"].name;
	
	// Our different registered scenes e.g. Mainmenu, Ingame, Options ...
	renderedScene[gameState["MainMenu"]] = unique_ptr<MainMenuScene>(new MainMenuScene());
	renderedScene[gameState["InGame"]] = unique_ptr<InGameScene>(new InGameScene());
}

/**
 * 
 * Scene destructor closes Image library.
 * 
 */
Scene::~Scene()
{
	TTF_Quit();
	IMG_Quit();
}

/**
 * 
 * Our render method that is called when a redraw is needed.
 * 
 */
void Scene::Redraw()
{
	if(SDL_SetRenderDrawColor(rendererPtr, 0, 0, 0, 255) != 0)
	{
		throw runtime_error(SDL_GetError());
	}

	if(SDL_RenderClear(rendererPtr) != 0)
	{
		throw runtime_error(SDL_GetError());
	}

	(*renderedScene[gameState[currentState]].get()).Draw();

	SDL_RenderPresent(rendererPtr);
}