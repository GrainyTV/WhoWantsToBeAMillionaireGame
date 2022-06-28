#ifndef KERDESEKVALASZOK_H
#define KERDESEKVALASZOK_H
#include <iostream>
using std::string;

class Kerdesek
{
	string category;
	int number;

	public:

	void setQuestion(Kerdesek* arr, int idx, int value, string category);

	int& getQuestionN(Kerdesek* arr, int idx);

	string& getQuestionC(Kerdesek* arr, int idx);
};

class Valaszok
{
	string answer;

	public:

	void setAnswer(Valaszok* arr, int idx, string value);

	string& getAnswer(Valaszok* arr, int idx);
};

#endif
