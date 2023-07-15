#ifndef MAINMENUSCENE_HPP
#define MAINMENUSCENE_HPP

#include <stdexcept>
#include "SDL.h"
#include "DrawableScene.hpp"
#include "Hexa.hpp"

using std::runtime_error;

class MainMenuScene : public DrawableScene
{
private:
	const Hexa startGame;
	SDL_Texture* startGameText;

	const Hexa options;
	SDL_Texture* optionsText;

	const Hexa quit;
	SDL_Texture* quitText;

	SDL_Texture* background;

	SDL_Texture* logo;

public:
	MainMenuScene();

	void Draw() const override;

	bool CheckTextboxPositionValidity() const;
};

#endif