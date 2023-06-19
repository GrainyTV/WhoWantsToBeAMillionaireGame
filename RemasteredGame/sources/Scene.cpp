#include "Scene.hpp"

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
	
	rendererPtr = renderer;
	invalidator.type = SDL_RegisterEvents(1);
	state = GameState::MainMenu;
	textureID = 0;
	
	FillTextureFiles();
	textures = LoadTextures();
}

/**
 * 
 * Scene destructor closes Image library.
 * 
 */
Scene::~Scene()
{
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

	SDL_RenderCopy(rendererPtr, textures[textureID], NULL, NULL);

	SDL_RenderPresent(rendererPtr);
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

/**
 * 
 * Fills our hashmap with the name of textures, categorized based on game state.
 * 
 */
void Scene::FillTextureFiles()
{
	deque<string> mainMenu = { "mainmenu_default.png", "mainmenu_1.png", "mainmenu_2.png", "mainmenu_3.png" };
	textureFiles[GameState::MainMenu] = mainMenu;

	deque<string> inGame = { "ingame_default.png", "ingame_1.png", "ingame_2.png", "ingame_3.png", "ingame_4.png" };
	textureFiles[GameState::InGame] = inGame;
}

/**
 * 
 * Getter function for game state.
 * return : state of the game
 * 
 */
GameState Scene::_State() const
{
	return state;
}

/**
 * 
 * Setter function for game state.
 * param value : new state
 * 
 */
void Scene::_State(const GameState& value)
{
	state = value;
	textureID = 0;
	UnloadTextures();
	textures = LoadTextures();
}

/**
 * 
 * Getter function for the ID of the texture.
 * return : currently rendered texture's ID
 * 
 */
unsigned int Scene::_TextureId() const
{
	return textureID;
}

/**
 * 
 * Setter function for the ID of the texture.
 * param value : new ID
 * 
 */
void Scene::_TextureId(const unsigned int& value)
{
	textureID = value;
}