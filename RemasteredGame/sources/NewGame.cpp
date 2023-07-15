#include "NewGame.hpp"

NewGame::NewGame(const deque<Data>& selected) : chosenQuestions(selected), currentQuestionIdx(0)
{
	if(UniqueCheck<Data>(chosenQuestions) == false)
	{
		throw invalid_argument("The number of unique questions do not match the required amount. Duplicate questions found.");
	}
}

Data NewGame::ThisRoundsData() const
{
	return chosenQuestions[currentQuestionIdx];
}

void NewGame::IterateToNextRound()
{
	++currentQuestionIdx;
}

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