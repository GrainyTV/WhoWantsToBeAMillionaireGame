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
		throw invalid_argument("None of the provided answers are equal to the solution for question: " + question);
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
 * Getter for difficulty
 * 
 */
Difficulty Data::_Difficulty()
{
	return diff;
}

/**
 * 
 * Print method for testing purposes
 * 
 */
void Data::Print()
{
	printf("%d %s %s %s %s %s %s\n", diff, question.c_str(), a.c_str(), b.c_str(), c.c_str(), d.c_str(), solution.c_str());
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
		throw invalid_argument(format("Provided difficulty ({0:c}) is neither E, M or H character.", input));
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
 * @return : wether the line is null or whitespace, or not
 * 
 */
bool Data::IsNullOrWhiteSpace(const string& str)
{
	return find_if(str.begin(), str.end(), [](unsigned char ch) { return !isspace(ch); } ) == str.end();
}

/**
 * 
 * Our main loading function that translates the singular lines of input to usable game data objects.
 * @param line : a singular line from an input file
 * @return : a game data object
 * 
 */
Data Data::RawInputToData(const string& line)
{
	// Separate properties in given line
	const deque<string> separatedLineValues = Split(line, '|');

	if(separatedLineValues.size() != 7)
	{
		throw invalid_argument(format("Line ({0:s}) does not consist of 7 elements and 6 separators \'|\'.", line));
	}

	// Difficulty
	const char difficulty = separatedLineValues[0][0];

	// Question
	const string question = separatedLineValues[1];

	// Answer A
	const string answerA = separatedLineValues[2];

	// Answer B
	const string answerB = separatedLineValues[3];

	// Answer C
	const string answerC = separatedLineValues[4];

	// Answer D
	const string answerD = separatedLineValues[5];

	// Solution
	const string solution = separatedLineValues[6];

	// Return in compact form
	return Data(difficulty, question, answerA, answerB, answerC, answerD, solution);
}

/**
 * 
 * Homemade string.Split function
 * @param str : the string to slice
 * @param delimiter : the character where the slicing happens
 * @return : a collection of tokens made from the original string
 * 
 */
deque<string> Data::Split(const string& str, const char& delimiter) 
{
	string token;
	deque<string> result;

	for(char letter : str)
	{
		if(letter == delimiter)
		{
			if(token.empty())
			{
				throw invalid_argument(format("Unnecessary separator \'|\' found in line: {0:s}.", str));
			}

			result.push_back(token);
			token.clear();
			continue;
		}
		
		token += letter;
	}

	if(token.empty() == false)
	{
		result.push_back(token);
	}
	
	return result;
}