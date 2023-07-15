#ifndef MAINMENUSCENE_HPP
#define MAINMENUSCENE_HPP

#include <stdexcept>
#include <deque>
#include <utility>
#include "SDL.h"
#include "DrawableScene.hpp"
#include "Hexa.hpp"

using std::runtime_error;
using std::invalid_argument;
using std::pair;
using std::make_pair;
using std::deque;

class MainMenuScene : public DrawableScene
{
private:
	deque<pair<Hexa, SDL_Texture*>> buttons;

	//const Hexa startGame;
	//SDL_Texture* startGameText;

	//const Hexa options;
	//SDL_Texture* optionsText;

	//const Hexa quit;
	//SDL_Texture* quitText;

	SDL_Texture* background;

	SDL_Texture* logo;

public:
	MainMenuScene();

	void Draw() const override;

	unsigned int Hit(SDL_Point mousePos) override;

	bool CheckTextboxPositionValidity() const;
};

#endif