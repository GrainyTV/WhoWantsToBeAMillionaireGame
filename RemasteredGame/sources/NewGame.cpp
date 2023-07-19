#include "NewGame.hpp"

/**
 * 
 * New game constructor checks uniqueness of the selected items and stores them if they are. Then sets question index to 0.
 * 
 */
NewGame::NewGame(const deque<Data>& selected) : chosenQuestions(selected)
{
	if(UniqueCheck<Data>(chosenQuestions) == false)
	{
		throw invalid_argument("The number of unique questions do not match the required amount. Duplicate questions found.");
	}

	checkIfGuessIsCorrect['A'] = [&, this] () { return ThisRoundsData()._A().compare(ThisRoundsData()._Solution()) == 0; };
	checkIfGuessIsCorrect['B'] = [&, this] () { return ThisRoundsData()._B().compare(ThisRoundsData()._Solution()) == 0; };
	checkIfGuessIsCorrect['C'] = [&, this] () { return ThisRoundsData()._C().compare(ThisRoundsData()._Solution()) == 0; };
	checkIfGuessIsCorrect['D'] = [&, this] () { return ThisRoundsData()._D().compare(ThisRoundsData()._Solution()) == 0; };
}

/**
 * 
 * Returns the data object of the current round.
 * @return : a singular data object
 * 
 */
Data NewGame::ThisRoundsData() const
{
	return chosenQuestions[currentQuestionIdx];
}

/**
 * 
 * Changes to next round after a successful one.
 * 
 */
void NewGame::IterateToNextRound()
{
	if(currentQuestionIdx + 1 < finalQuestion)
	{
		++currentQuestionIdx;
	}
}

bool NewGame::GuessAnswer(char answerLetter)
{
	bool result = (checkIfGuessIsCorrect[answerLetter]());

	if(result == true)
	{
		IterateToNextRound();
	}

	return result;
}

unsigned int NewGame::_Round() const
{
	return currentQuestionIdx;
}

/**
 * 
 * Generic static function to check wether a collection of items are unique.
 * @param : a deque of type T
 * @return : either true if they are unique or false
 * 
 */
template<class T>
bool NewGame::UniqueCheck(deque<T> list)
{
	set<T> uniqueCount;

	for(auto element : list)
	{
		uniqueCount.insert(element);
	}

	return (uniqueCount.size() == list.size()) ? true : false;
}