#ifndef NEWGAME_HPP
#define NEWGAME_HPP

#include <deque>
#include <set>
#include <algorithm>
#include <stdexcept>
#include "Data.hpp"

using std::deque;
using std::set;
using std::invalid_argument;

class NewGame
{
private:
	// Which question the player if currently on
	int currentQuestionIdx;

	// The selected questions for this run
	deque<Data> chosenQuestions;

public:
	NewGame(const deque<Data>& selected);

	Data ThisRoundsData() const;

	void IterateToNextRound();

	template<class T>
	static bool UniqueCheck(deque<T> list);
};

#endif