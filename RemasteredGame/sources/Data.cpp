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
void Data::LoadContent(string fileName)
{
	ifstream reader;
	reader.exceptions(ifstream::badbit);
	
	try
	{
		reader.open("text/" + fileName, ios::in);
		
		do
		{
			string line;
			getline(reader, line);
		
		} while(reader.eof() == false);
	}
	catch(const ifstream::failure& e)
	{
		printf("%s\n", "Exception with opening / reading the file!");
	}

	reader.close();
}