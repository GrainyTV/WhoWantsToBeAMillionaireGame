#include "Scene.hpp"

random_device Scene::randomGenerator = random_device();

/**
 * 
 * Constructor with the renderer acquired.
 * @param renderer : renderer received from window
 * 
 */
Scene::Scene(SDL_Renderer* renderer) : rendererPtr(renderer)
{
	invalidator.type = SDL_RegisterEvents(1);
	state = GameState::MainMenu;
	textureID = 0;
	FillTextureFiles();
	textures = LoadCurrentTextures(state);
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

deque<SDL_Texture*> Scene::LoadCurrentTextures(const GameState& currentState)
{
	deque<SDL_Texture*> result;

	for(auto file : textureFiles[currentState])
	{
		path fileWithDirectory = "textures";
		fileWithDirectory /= file;
		SDL_Texture* loaded = IMG_LoadTexture(rendererPtr, fileWithDirectory.c_str());

		if(loaded == NULL)
		{
			throw runtime_error(format("Could not load texture: {0:s}", fileWithDirectory.string()));
		}

		result.push_back(loaded);
	}

	return result;
}

void Scene::FillTextureFiles()
{
	deque<string> mainMenu = { "mainmenu_default.png", "mainmenu_1.png", "mainmenu_2.png", "mainmenu_3.png" };
	textureFiles[GameState::MainMenu] = mainMenu;

	deque<string> inGame = { "ingame_default.png" };
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

/**
 * 
 * Non-deterministic seeded random number generator.
 * @param lowerBound : inclusive, integer lower bound
 * @param upperBound : inclusive, integer upper bound
 * @return : the chosen integer from the interval
 * 
 */
int Scene::Random(int lowerBound, int upperBound)
{
	// seeding the engine:
	mt19937 engine{ randomGenerator() }; 
	uniform_int_distribution<int> distribution{ lowerBound, upperBound };

	// get random numbers with:
	return distribution(engine);
}