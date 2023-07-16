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
Scene::Scene(SDL_Renderer* renderer) : hitId(0), rendererPtr(renderer)
{
	if(IMG_Init(IMG_INIT_PNG) == 0)
	{
		throw runtime_error("Failed to load Image library.");
	}

	if(TTF_Init() != 0)
	{
		throw runtime_error(TTF_GetError());
	}

	gameState.Add("MainMenu");
	gameState.Add("InGame");
	currentState = gameState["MainMenu"].name;
	
	// Our different registered scenes e.g. Mainmenu, Ingame, Options ...
	renderedScene[gameState["MainMenu"]] = unique_ptr<MainMenuScene>(new MainMenuScene(this));
	renderedScene[gameState["InGame"]] = unique_ptr<InGameScene>(new InGameScene());

	// Our different state changes
	stateChange["InGame"] = [&, this] ()
	{
		(*static_cast<InGameScene*>(renderedScene[gameState[currentState]].get())).InitiateNewGame(); 
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

/**
 * 
 * Helper function for tracking wether the mouse cursor entered a new location compared to the previous one.
 * @param mousePos : the location of the cursor
 * @return : wether the mouse entered a new area
 * 
 */
bool Scene::CheckForHit(SDL_Point mousePos)
{
	unsigned int newId = (*renderedScene[gameState[currentState]].get()).Hit(mousePos);
		
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
	(*renderedScene[gameState[currentState]].get()).Invoke(hitId - 1);
	return true;
}

void Scene::ChangeScene(const string& which)
{
	currentState = gameState[which].name;
	stateChange[currentState]();
}