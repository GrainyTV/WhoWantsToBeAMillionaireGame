#ifndef MAINMENUSCENE_HPP
#define MAINMENUSCENE_HPP

#include "DrawableScene.hpp"

using std::make_tuple;

class MainMenuScene : public DrawableScene
{
private:
	// Logo image
	SDL_Texture* logo;

public:
	MainMenuScene();

	void Draw() const override;

	bool CheckTextboxPositionValidity() const;
};

#endif