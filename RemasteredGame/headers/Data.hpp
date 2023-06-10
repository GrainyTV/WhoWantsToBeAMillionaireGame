#ifndef DATA_HPP
#define DATA_HPP

#include <string>
#include <stdexcept>

using std::invalid_argument;
using std::string;

enum Difficulty { Easy, Medium, Hard };

class Data
{
private:
	// Difficulty (E - easy, M - medium, H - hard)
	Difficulty diff;

	// The given question or statement
	string question;

	// Answer A
	string a;

	// Answer B
	string b;

	// Answer C
	string c;

	// Answer D
	string d;

	// The corresponding solution to the question (either A, B, C or D)
	string solution;

public:

	Data(char difficulty, string question, string a, string b, string c, string d, string solution);

	static Difficulty CharToDifficulty(char input);

	static void LoadContent(string fileName);
};

#endif