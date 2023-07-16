#ifndef INGAMESCENE_HPP
#define INGAMESCENE_HPP

#include <stdexcept>
#include <memory>
#include <deque>
#include <tuple>
#include <functional>
#include "SDL.h"
#include "Hexa.hpp"
#include "DrawableScene.hpp"
#include "NewGame.hpp"
#include "Line.hpp"

using std::runtime_error;
using std::unique_ptr;
using std::make_unique;
using std::tuple;
using std::make_tuple;
using std::get;
using std::function;

class InGameScene : public DrawableScene
{
private:
	tuple<Hexa, SDL_Texture*> question;

	deque<tuple<Hexa, SDL_Texture*, function<void()>>> buttons;

	SDL_Texture* background;

	// Lines under each of the buttons
	deque<Line> buttonLines;

	// Width of the lines
	const int lineWidth;

	// Color of the lines
	const SDL_Color BLUE;

	/*const Hexa question;
	SDL_Texture* questionText;

	const Hexa answer_a;
	SDL_Texture* answer_aText;
	
	const Hexa answer_b;
	SDL_Texture* answer_bText;
	
	const Hexa answer_c;
	SDL_Texture* answer_cText;
	
	const Hexa answer_d;
	SDL_Texture* answer_dText; */

	// All required data for the game
	// Initially null, until the user launches one
	unique_ptr<NewGame> currentGame; 

public:
	InGameScene();

	void Draw() const override;

	unsigned int Hit(SDL_Point mousePos) override;

	bool CheckTextboxPositionValidity() const;

	void InitiateNewGame();

	void Invoke(int index) const override;
};

#endif