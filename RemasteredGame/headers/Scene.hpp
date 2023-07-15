#ifndef SCENE_HPP
#define SCENE_HPP

#include <stdexcept>
#include <unordered_map>
#include <deque>
#include <filesystem>
#include <format>
#include <future>
#include <memory>
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "DrawableScene.hpp"
#include "Enumeration.hpp"

using std::runtime_error;
using std::string;
using std::unordered_map;
using std::deque;
using std::filesystem::path;
using std::format;
using std::async;
using std::launch;
using std::future;
using std::unique_ptr;

class Scene
{
private:
	// Possible scenes of the application
	Enumeration gameState;

	// The currently applied scene
	string currentState;

	// Copy of the renderer object
	SDL_Renderer* rendererPtr;

	// Our different renderable scenes
	unordered_map<EnumField, unique_ptr<DrawableScene>, EnumField::Hash> renderedScene;

public:
	Scene(SDL_Renderer* renderer);

	~Scene();

	void Redraw();

	//deque<SDL_Texture*> LoadTextures();

	//deque<SDL_Surface*> LoadSurfaces(const GameState& currentState);
};

#endif