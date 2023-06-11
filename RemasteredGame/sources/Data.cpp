#include "Data.hpp"

/**
 * 
 * Constructor for a singular Data object. Throws exception if the solution value is invalid.
 * @param difficulty : a char value representing a difficulty level
 * @param question : the given question or statement
 * @param a : answer A
 * @param b : answer B
 * @param c : answer C
 * @param d : answer D
 * @param solution : equals to A, B, C, or D
 * 
 */
Data::Data(char difficulty, string question, string a, string b, string c, string d, string solution)
{
	if (solution.compare(a) != 0 && solution.compare(b) != 0 && 
		solution.compare(c) != 0 && solution.compare(d) != 0)
	{
		throw invalid_argument("None of the provided answers are equal to the solution!");
	}

	this->diff = CharToDifficulty(difficulty);
	this->question = question;
	this->a = a;
	this->b = b;
	this->c = c;
	this->d = d;
	this->solution = solution;
}

/**
 * 
 * A helper method to convert a char value to explicit difficulty. If the input is not E, M, or H, it throws an exception.
 * @param input : the char that should represent a difficulty level
 * @return : the converted difficulty or invalid argument exception
 * 
 */
Difficulty Data::CharToDifficulty(char input)
{
	if(input != 'E' && input != 'M' && input != 'H')
	{
		throw invalid_argument("Provided difficulty is neither E, M or H character!");
	}

	return
		(input == 'E') ? Difficulty::Easy : 
		(input == 'M') ? Difficulty::Medium : 
		Difficulty::Hard;
}

/**
 * 
 * Loads the contents of the given parameter file, unless the file is not found or improperly formatted.
 * @param fileName : the name of the given input file with extensions
 * 
 */
deque<Data> Data::LoadContent(const string& fileName)
{
	ifstream reader;

	path fileWithDirectory = "text";
	fileWithDirectory /= fileName;

	printf("%s\n", fileWithDirectory.c_str());
	const string file = fileWithDirectory.string();

	reader.open(file, ios::in);

	if(reader.fail())
	{
		throw ios::failure("Cannot open input file: " + fileWithDirectory.string());
	}
	
	deque<Data> result;

	do
	{
		string line;
		getline(reader, line);

		if(IsNullOrWhiteSpace(line) == false)
		{
			result.push_back(Data::RawInputToData(line));
		}
	
	} while(reader.eof() == false);

	return result;
}

/**
 * 
 * Helps to identify empty or null lines from the input file.
 * @param str : the line to execute the function on
 * 
 */
bool Data::IsNullOrWhiteSpace(string const& str)
{
	return find_if(str.begin(), str.end(), [](unsigned char ch) { return !isspace(ch); } ) == str.end();
}

Data Data::RawInputToData(const string& line)
{
	// Difficulty
	const char difficulty = line[0];

	// Question
	const int questionMarkPosition = line.find_first_of('?');
	const string question = line.substr(2, questionMarkPosition - 1);

	// All answers
	const int answerPosition = line.find("Válaszok:") + string("Válaszok:").size();
	int solutionPosition = line.find("Megoldás:");
	const string allAnswers = line.substr(answerPosition, solutionPosition - 2 - answerPosition);
	const deque<string> separatedAnswers = Split(allAnswers, ',');

	// Answer A
	const string answerA = separatedAnswers[0].substr(1); 

	// Answer B
	const string answerB = separatedAnswers[1].substr(1);

	// Answer C
	const string answerC = separatedAnswers[2].substr(1);

	// Answer D
	const string answerD = separatedAnswers[3].substr(1);

	// Solution
	solutionPosition = solutionPosition + string("Megoldás: ").size();
	const string solution = line.substr(solutionPosition, line.size() - solutionPosition);

	printf("%c %s %s %s %s %s %s\n", difficulty, question.c_str(), answerA.c_str(), answerB.c_str(), answerC.c_str(), answerD.c_str(), solution.c_str());

	return Data(difficulty, question, answerA, answerB, answerC, answerD, solution);
}


deque<string> Data::Split(const string& s, const char& delimiter) 
{
	int pos_start = 0, pos_end;
	string token;
	deque<string> result;

	for(char letter : s)
	{
		if(letter == delimiter)
		{
			result.push_back(token);
			token.clear();
			continue;
		}
		
		token += letter;
	}

	if(result.empty() == false)
	{
		result.push_back(token);
	}
	
	return result;
}