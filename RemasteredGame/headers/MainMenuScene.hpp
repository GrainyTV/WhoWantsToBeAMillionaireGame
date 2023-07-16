#ifndef MAINMENUSCENE_HPP
#define MAINMENUSCENE_HPP

#include <stdexcept>
#include <deque>
#include <utility>
#include "SDL.h"
#include "DrawableScene.hpp"
#include "Hexa.hpp"
#include "Line.hpp"

using std::runtime_error;
using std::invalid_argument;
using std::pair;
using std::make_pair;
using std::deque;

class MainMenuScene : public DrawableScene
{
private:
	//
	// Our clickable buttons
	// 0 - New Game
	// 1 - Options
	// 2 - Quit
	//
	deque<pair<Hexa, SDL_Texture*>> buttons;

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

public:
	MainMenuScene();

	void Draw() const override;

	unsigned int Hit(SDL_Point mousePos) override;

	bool CheckTextboxPositionValidity() const;
};

#endif