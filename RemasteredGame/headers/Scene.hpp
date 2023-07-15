#ifndef SCENE_HPP
#define SCENE_HPP

#include <stdexcept>
#include <unordered_map>
#include <memory>
#include "SDL.h"
#include "DrawableScene.hpp"
#include "Enumeration.hpp"

using std::runtime_error;
using std::string;
using std::unordered_map;
using std::unique_ptr;

class Scene
{
private:
	// Possible scenes of the application
	Enumeration gameState;

	// The name of the currently applied scene
	string currentState;

	// Copy of the renderer object
	SDL_Renderer* rendererPtr;

	// The contents of our different renderable scenes
	unordered_map<EnumField, unique_ptr<DrawableScene>, EnumField::Hash> renderedScene;

public:
	Scene(SDL_Renderer* renderer);

	~Scene();

	void Redraw();
};

#endif