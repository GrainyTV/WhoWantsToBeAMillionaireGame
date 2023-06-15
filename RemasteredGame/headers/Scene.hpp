#ifndef SCENE_HPP
#define SCENE_HPP

#include <stdexcept>
#include <random>
#include <unordered_map>
#include <deque>
#include <filesystem>
#include <format>
#include <future>
#include "SDL.h"
#include "SDL_image.h"

using std::runtime_error;
using std::random_device;
using std::mt19937;
using std::uniform_int_distribution;
using std::string;
using std::unordered_map;
using std::deque;
using std::filesystem::path;
using std::format;
using std::async;
using std::launch;
using std::future;

#include <thread>

enum GameState { MainMenu, InGame, Options };

class Scene
{
private:
	// Copy of the renderer object
	SDL_Renderer* rendererPtr;

	// Event to call when screen needs redraw
	SDL_Event invalidator;

	// Our loaded textures [GameState(key) - [ID(key) - Texture(value)]]
	deque<SDL_Texture*> textures;

	// Our texture files
	unordered_map<GameState, deque<string>> textureFiles;

	// The current state of the game
	GameState state;

	// The current ID of rendered texture
	unsigned int textureID;

	// Expensive non-deterministic seed for random numbers
	static random_device randomGenerator;

public:
	Scene(SDL_Renderer* renderer);

	~Scene();

	void Redraw();

	void Invalidate();

	deque<SDL_Texture*> LoadTextures();

	deque<SDL_Surface*> LoadSurfaces(const GameState& currentState);

	void UnloadTextures();

	void FillTextureFiles();

	GameState _State() const;

	void _State(const GameState& value);

	unsigned int _TextureId() const;

	void _TextureId(const unsigned int& value);

	static int Random(int lowerBound, int upperBound);
};

#endif