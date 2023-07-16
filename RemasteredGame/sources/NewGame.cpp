#include "NewGame.hpp"

/**
 * 
 * New game constructor checks uniqueness of the selected items and stores them if they are. Then sets question index to 0.
 * 
 */
NewGame::NewGame(const deque<Data>& selected) : chosenQuestions(selected), currentQuestionIdx(0)
{
	if(UniqueCheck<Data>(chosenQuestions) == false)
	{
		throw invalid_argument("The number of unique questions do not match the required amount. Duplicate questions found.");
	}
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
	++currentQuestionIdx;
}

void NewGame::GuessAnswer(char answerLetter)
{
    switch(answerLetter)
    {
    case 'A':
        break;
    case 'B':
        break;
    case 'C':
        break;
    case 'D':
        break;
    default:
        break;
    }
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