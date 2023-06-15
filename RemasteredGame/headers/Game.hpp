#ifndef GAME_HPP
#define GAME_HPP

#include <locale>
#include <string>
#include <unordered_map>
#include <deque>
#include <memory>
#include "Data.hpp"
#include "GameView.hpp"

using std::unordered_map;
using std::deque;
using std::string;
using std::unique_ptr;

enum Category { Art, Astronomy, Food, Geography, History, Holidays, Literature, Music, Nature, Other, Proverbs, TelevisionSeries, Sports, Tabloid, Theatre, Transport };

class Game
{
private:
	// Input files for categories [category(K) - fileName(V)]
	unordered_map<Category, string> fileNames;

	// Loaded data represented in sorted way [category(K) - [difficulty(K) - question(V)]]
	unordered_map<Category, unordered_map<Difficulty, deque<Data>>> gameData;

	// Constructor hidden from outside
	Game();

	// Copy constructor not needed
	Game(Game const&) = delete;

	// Equals operator not needed either
	void operator=(Game const&) = delete;

	// The application's frontend
	unique_ptr<GameView> frontEnd;

	// Singleton instance for the game
	static unique_ptr<Game> instance;

	// Indicator wether the game is still running
	static bool running;

public:
	void StoreData();

	void FillFileNames();

	// Test

	void Testing();

	// Test

	void Launch();

	static Game* Instance();

	static void Terminate();

	static bool IsRunning();
};

#endif