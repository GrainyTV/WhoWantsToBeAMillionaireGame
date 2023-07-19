#ifndef INGAMESCENE_HPP
#define INGAMESCENE_HPP

#include <memory>
#include <thread>
#include <chrono>
#include "DrawableScene.hpp"
#include "NewGame.hpp"

using std::make_tuple;
using std::make_unique;
using std::thread;
using std::this_thread::sleep_for;
using std::literals::chrono_literals::operator""ms;

enum class GameState { Neutral, Won, Lost };

class InGameScene : public DrawableScene
{
private:
	// Panel for question is separated from our buttons
	// Because it does not have any functionality
	tuple<Hexa, SDL_Texture*> question;

	// All required data for a singular game instance
	// Initially null, until the user launches one
	unique_ptr<NewGame> currentGame; 

	// Tracking for game's current state
	GameState gameState { GameState::Neutral };

public:
	InGameScene();

	void Draw() const override;

	bool CheckTextboxPositionValidity() const;

	void InitiateNewGame();

	void BeginGuessOnAnswer(char which);

	void NewRound();
};

#endif