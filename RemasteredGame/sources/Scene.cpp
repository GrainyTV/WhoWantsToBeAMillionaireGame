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

/*

deque<SDL_Texture*> Scene::LoadTextures()
{
	deque<SDL_Texture*> result;

	future<deque<SDL_Surface*>> loadingTask = async(launch::async, &Scene::LoadSurfaces, this, state);
	deque<SDL_Surface*> loadedSurfaces = loadingTask.get();

	while(loadedSurfaces.empty() == false)
	{
		SDL_Surface* surface = loadedSurfaces.front();
		SDL_Texture* texture = SDL_CreateTextureFromSurface(rendererPtr, surface);

		if(texture == NULL)
		{
			throw runtime_error("Could not convert surface to texture.");
		}

		result.push_back(texture);
		SDL_FreeSurface(surface);
		loadedSurfaces.pop_front();
	}

	return result;
}

/**
 * 
 * Loads the textures required by the given game state object.
 * e.g. all the mainmenu textures if the state is MainMenu
 * @param currentState : the current state of the game
 * @returns : a collection of texture objects
 * 
 */

/*
deque<SDL_Surface*> Scene::LoadSurfaces(const GameState& currentState)
{
	deque<SDL_Surface*> result;

	for(auto file : textureFiles[currentState])
	{
		path fileWithDirectory = "textures";
		fileWithDirectory /= file;

		SDL_Surface* loaded = IMG_Load(fileWithDirectory.c_str());

		if(loaded == NULL)
		{
			throw runtime_error(format("Could not load texture: {0:s}", fileWithDirectory.string()));
		}

		result.push_back(loaded);
	}

	return result;
}

void Scene::UnloadTextures()
{
	while(textures.empty() == false)
	{
		SDL_Texture* texture = textures.front();
		SDL_DestroyTexture(texture);
		textures.pop_front();
	}
}

*/