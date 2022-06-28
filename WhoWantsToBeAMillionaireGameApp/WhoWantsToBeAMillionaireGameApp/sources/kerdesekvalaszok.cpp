#include "kerdesekvalaszok.h"

void Kerdesek::setQuestion(Kerdesek* arr, int idx, int value, string category)
{
	arr[idx].number = value;
	arr[idx].category = category;
}

int& Kerdesek::getQuestionN(Kerdesek* arr, int idx)
{
	return arr[idx].number;
}

string& Kerdesek::getQuestionC(Kerdesek* arr, int idx)
{
	return arr[idx].category;
}

void Valaszok::setAnswer(Valaszok* arr, int idx, string value)
{
	arr[idx].answer = value;
}

string& Valaszok::getAnswer(Valaszok* arr, int idx)
{
	return arr[idx].answer;
}