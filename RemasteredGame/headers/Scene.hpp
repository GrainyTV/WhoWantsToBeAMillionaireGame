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
using std::make_unique;
using std::string;
using std::function;

class Scene
{
private:
	// Observer for mouse position
	unsigned int hitId { 0 };

	// Event to call when screen needs redraw
	SDL_Event invalidator { .type = SDL_RegisterEvents(1) };

	// Copy of renderer from Window class
	SDL_Renderer* renderer { Window::_Renderer() };

	// Possible scenes of the application
	Enumeration gameState { Enumeration({ "MainMenu", "InGame" }) };

	// The name of the currently applied scene
	EnumField currentState { gameState["MainMenu"] };

	// The currently applied renderable scene
	unique_ptr<DrawableScene> renderedScene;

	// Delegate for running methods after changing scenes
	unordered_map<EnumField, function<void()>, EnumField::Hash> stateChange;

	// Constructor hidden from the outside
	Scene();

public:

	// Copy constructor not needed
	Scene(const Scene&) = delete;

	// Equals operator not needed either
	void operator=(const Scene&) = delete;

	void Invalidate();

	void Redraw();

	bool CheckForHit(SDL_Point mousePos);

	bool ClickOnCurrentHitId();

	void ChangeScene(const string& which);

	static Scene& Instance();
};

#endif