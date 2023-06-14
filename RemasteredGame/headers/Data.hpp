#ifndef DATA_HPP
#define DATA_HPP

#include <string>
#include <stdexcept>
#include <fstream>
#include <algorithm>
#include <deque>
#include <ios>
#include <filesystem>
#include <format>

using std::invalid_argument;
using std::string;
using std::ifstream;
using std::ios;
using std::find_if;
using std::deque;
using std::filesystem::path;
using std::format;

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

	Difficulty _Difficulty() const;

	void Print();

	static Difficulty CharToDifficulty(const char& input);

	static deque<Data> LoadContent(const string& fileName);

	static bool IsNullOrWhiteSpace(const string& str);

	static Data RawInputToData(const string& line);

	static deque<string> Split(const string& str, const char& delimiter);
};

#endif