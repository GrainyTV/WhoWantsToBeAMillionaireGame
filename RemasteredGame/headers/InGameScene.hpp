#ifndef INGAMESCENE_HPP
#define INGAMESCENE_HPP

#include <memory>
#include "DrawableScene.hpp"
#include "NewGame.hpp"

using std::make_tuple;
using std::make_unique;

class InGameScene : public DrawableScene
{
private:
	// Panel for question is separated from our buttons
	// Because it does not have any functionality
	tuple<Hexa, SDL_Texture*> question;

	// All required data for a singular game instance
	// Initially null, until the user launches one
	unique_ptr<NewGame> currentGame; 

public:
	InGameScene();

	void Draw() const override;

	bool CheckTextboxPositionValidity() const;

	void InitiateNewGame();
};

#endif