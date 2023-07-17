#ifndef SCENE_HPP
#define SCENE_HPP

#include <stdexcept>
#include <unordered_map>
#include <memory>
#include <string>
#include <functional>
#include "SDL.h"
#include "Window.hpp"
#include "DrawableScene.hpp"
#include "Enumeration.hpp"

using std::runtime_error;
using std::string;
using std::unordered_map;
using std::unique_ptr;
using std::string;
using std::function;

class Scene
{
private:
	// Observer for mouse position
	unsigned int hitId { 0 };

	// Copy of renderer from Window class
	SDL_Renderer* renderer { Window::_Renderer() };

	// Possible scenes of the application
	Enumeration gameState { Enumeration({ "MainMenu", "InGame" }) };

	// The name of the currently applied scene
	EnumField currentState { gameState["MainMenu"] };

	// The contents of our different renderable scenes
	unordered_map<EnumField, unique_ptr<DrawableScene>, EnumField::Hash> renderedScene;

	// Delegate for running methods after changing scenes
	unordered_map<EnumField, function<void()>, EnumField::Hash> stateChange;

public:
	Scene();

	~Scene();

	void Redraw();

	bool CheckForHit(SDL_Point mousePos);

	bool ClickOnCurrentHitId();

	void ChangeScene(const string& which);
};

#endif