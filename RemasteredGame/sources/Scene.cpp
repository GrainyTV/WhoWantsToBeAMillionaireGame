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
Scene::Scene()
{
	if(IMG_Init(IMG_INIT_PNG) == 0)
	{
		throw runtime_error("Failed to load Image library.");
	}

	if(TTF_Init() != 0)
	{
		throw runtime_error(TTF_GetError());
	}
	
	// Our different registered scenes e.g. Mainmenu, Ingame, Options ...
	renderedScene[gameState["MainMenu"]] = unique_ptr<MainMenuScene>(new MainMenuScene());
	renderedScene[gameState["InGame"]] = unique_ptr<InGameScene>(new InGameScene());

	// Our different state changes
	stateChange[gameState["InGame"]] = [&, this] ()
	{
		(*static_cast<InGameScene*>(renderedScene[currentState].get())).InitiateNewGame(); 
	};
}

/**
 * 
 * Scene destructor closes Image and TTF library.
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
	if(SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255) != 0)
	{
		throw runtime_error(SDL_GetError());
	}

	if(SDL_RenderClear(renderer) != 0)
	{
		throw runtime_error(SDL_GetError());
	}

	(*renderedScene[currentState].get()).Draw();

	SDL_RenderPresent(renderer);
}

/**
 * 
 * Helper function for tracking wether the mouse cursor entered a new location compared to the previous one.
 * @param mousePos : the location of the cursor
 * @return : wether the mouse entered a new area
 * 
 */
bool Scene::CheckForHit(SDL_Point mousePos)
{
	unsigned int newId = (*renderedScene[currentState].get()).Hit(mousePos);
		
	if(newId == hitId)
	{
		return false;
	}

	hitId = newId;
	return true;
}

bool Scene::ClickOnCurrentHitId()
{
	if(hitId == 0)
	{
		// Mouse is on empty surface
		return false;
	}

	// Array indexing starts from 0 so we subtract 1
	(*renderedScene[currentState].get()).Invoke(hitId - 1);
	return true;
}

void Scene::ChangeScene(const string& which)
{
	currentState = gameState[which];
	stateChange[currentState]();
}