#ifndef GAME_HPP
#define GAME_HPP

#include <locale>
#include <string>
#include <unordered_map>
#include <deque>
#include <memory>
#include "Data.hpp"

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

	// Singleton instance for the game
	static unique_ptr<Game> instance;

	// Constructor hidden from outside
	Game();

	// Copy constructor not needed
	Game(Game const&) = delete;

	// Equals operator not needed either
	void operator=(Game const&) = delete;

public:
	void Run();

	void FillFileNames();

	// Test

	void Testing();

	// Test

	static Game* Instance();
};

#endif