#ifndef INGAMESCENE_HPP
#define INGAMESCENE_HPP

#include <memory>
#include <thread>
#include <chrono>
#include <mutex>
#include <condition_variable>
#include <deque>
#include <stdexcept>
#include "DrawableScene.hpp"
#include "NewGame.hpp"

using std::make_tuple;
using std::make_unique;
using std::thread;
using std::this_thread::sleep_for;
using std::literals::chrono_literals::operator""ms;
using std::deque;
using std::mutex;
using std::condition_variable;
using std::lock_guard;
using std::unique_lock;
using std::runtime_error;

enum class GameState { Neutral, Won, Lost };

class InGameScene : public DrawableScene
{
private:
	// Panel for question is separated from our buttons
	// Because it does not have any functionality
	tuple<Hexa, SDL_Texture*, bool> question;

	// All required data for a singular game instance
	// Initially null, until the user launches one
	unique_ptr<NewGame> currentGame; 

	// Tracking for game's current state
	GameState gameState { GameState::Neutral };

	// A temporary container that holds the new textures until
	// all animation stops
	deque<SDL_Texture*> textureHolder;

	// The letter of the guessed answer
	char guessedLetterIdx;

	mutex animationMutex;

	condition_variable lockHandler;

	bool answerValidationAnimationFinished { true };

public:
	InGameScene();

	void Draw() const override;

	bool CheckTextboxPositionValidity() const;

	void InitiateNewGame();

	void BeginGuessOnAnswer(char guessedLetter);

	void NewRound();

	void AnswerValidationAnimation(bool success);

	void RunGarbageCollection();

	void LoadTextures();
};

#endif