#ifndef INGAMESCENE_HPP
#define INGAMESCENE_HPP

#include <initializer_list>
#include <stdexcept>
#include <memory>
#include <map>
#include "SDL.h"
#include "Hexa.hpp"
#include "DrawableScene.hpp"
#include "NewGame.hpp"

using std::initializer_list;
using std::runtime_error;
using std::format;
using std::unique_ptr;
using std::make_unique;
using std::map;

class InGameScene : public DrawableScene
{
private:
	const Hexa question;
	SDL_Texture* questionText;

	const Hexa answer_a;
	SDL_Texture* answer_aText;
	
	const Hexa answer_b;
	SDL_Texture* answer_bText;
	
	const Hexa answer_c;
	SDL_Texture* answer_cText;
	
	const Hexa answer_d;
	SDL_Texture* answer_dText;

	// All required data for the game
	// Initially null, until the user launches one
	unique_ptr<NewGame> currentGame; 

public:
	InGameScene();

	void Draw() const override;

	unsigned int Hit(SDL_Point mousePos) override;

	bool CheckTextboxPositionValidity() const;

	void InitiateNewGame();
};

#endif