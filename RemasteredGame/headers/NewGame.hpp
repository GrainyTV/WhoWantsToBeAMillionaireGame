#ifndef NEWGAME_HPP
#define NEWGAME_HPP

#include <deque>
#include <set>
#include <algorithm>
#include <stdexcept>
#include <unordered_map>
#include <functional>
#include "Data.hpp"

using std::deque;
using std::set;
using std::invalid_argument;
using std::unordered_map;
using std::function;

class NewGame
{
private:
	// Number of questions
	static constexpr int finalQuestion { 15 };

	// Which question the player if currently on
	int currentQuestionIdx { 0 };

	// The selected questions for this run
	deque<Data> chosenQuestions;

	unordered_map<char, function<bool()>> checkIfGuessIsCorrect;

	void IterateToNextRound();

public:
	NewGame(const deque<Data>& selected);

	Data ThisRoundsData() const;

	bool GuessAnswer(char answerLetter);

	unsigned int _Round() const;

	template<class T>
	static bool UniqueCheck(deque<T> list);
};

#endif