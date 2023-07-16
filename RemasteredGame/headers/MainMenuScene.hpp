#ifndef MAINMENUSCENE_HPP
#define MAINMENUSCENE_HPP

#include <stdexcept>
#include <deque>
#include <tuple>
#include <functional>
#include "SDL.h"
#include "Scene.hpp"
#include "DrawableScene.hpp"
#include "Hexa.hpp"
#include "Line.hpp"

using std::runtime_error;
using std::invalid_argument;
using std::tuple;
using std::make_tuple;
using std::get;
using std::deque;
using std::function;

class MainMenuScene : public DrawableScene
{
private:
	// Our clickable buttons
	// They consist of a hexagon, a texture of text and a backend function call
	// {0:New Game}, {1:Options}, {2:Quit}
	deque<tuple<Hexa, SDL_Texture*, function<void()>>> buttons;

	// Lines under each of the buttons
	deque<Line> buttonLines;

	// Width of the lines
	const int lineWidth;

	// Color of the lines
	const SDL_Color BLUE;

	// Background image
	SDL_Texture* background;

	// Logo image
	SDL_Texture* logo;

	// Copy of scene pointer
	Scene* scenePtr;

public:
	MainMenuScene(Scene* scene);

	void Draw() const override;

	unsigned int Hit(SDL_Point mousePos) override;

	bool CheckTextboxPositionValidity() const;

	void Invoke(int index) const override;
};

#endif