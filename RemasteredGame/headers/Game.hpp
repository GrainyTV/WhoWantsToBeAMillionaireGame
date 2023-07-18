#ifndef GAME_HPP
#define GAME_HPP

#include <locale>
#include <string>
#include <unordered_map>
#include <deque>
#include <memory>
#include <filesystem>
#include <algorithm>
#include "Data.hpp"
#include "GameLoop.hpp"
#include "Enumeration.hpp"

using std::unordered_map;
using std::deque;
using std::string;
using std::unique_ptr;
using std::filesystem::path;
using std::filesystem::directory_iterator;
using std::find;

class Game
{
private:
	// Dynamically changeable enum++ class with count property
	Enumeration category;

	// Input files for categories [category(K) - fileName(V)]
	unordered_map<EnumField, string, EnumField::Hash> fileNames;

	// Loaded data represented in sorted way [category(K) - [difficulty(K) - data(V)]]
	unordered_map<EnumField, unordered_map<Difficulty, deque<Data>>, EnumField::Hash> gameData;

	// Constructor hidden from outside
	Game();

	// Copy constructor not needed
	Game(Game const&) = delete;

	// Equals operator not needed either
	void operator=(Game const&) = delete;

	// The application's frontend
	unique_ptr<GameLoop> frontEnd;

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

	deque<Data> GenerateNewGame();

	static Game* Instance();

	static void Terminate();

	static bool IsRunning();	
};

#endif