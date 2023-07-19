#include "Scene.hpp"
#include "MainMenuScene.hpp"
#include "InGameScene.hpp"

/**
 * 
 * Constructor for a generic scene class.
 * 
 */
Scene::Scene() : renderedScene(make_unique<MainMenuScene>())
{
	// Our different state changes
	stateChange[gameState["InGame"]] = [&, this] ()
	{
		renderedScene = make_unique<InGameScene>();
		//InGameScene newScene = (*static_cast<InGameScene*>(renderedScene.get()));
		//auto newSceneTask = async(launch::async, &InGameScene::InitiateNewGame, &newScene);
		thread createNewScene { [&, this] () { (*static_cast<InGameScene*>(renderedScene.get())).InitiateNewGame(); } };
		createNewScene.detach();
	};
}

/**
 * 
 * Method to push an invalidate call onto the event stack.
 * 
 */
void Scene::Invalidate()
{
	SDL_PushEvent(&invalidator);
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

	(*renderedScene.get()).Draw();

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
	unsigned int newId = (*renderedScene.get()).Hit(mousePos);
		
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
	(*renderedScene.get()).Invoke(hitId - 1);
	return true;
}

void Scene::ChangeScene(const string& which)
{
	currentState = gameState[which];
	stateChange[currentState]();
}

Scene& Scene::Instance()
{
	static Scene sceneInstance;
	
	return sceneInstance;
}