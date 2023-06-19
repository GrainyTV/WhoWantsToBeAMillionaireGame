#ifndef CATEGORYCHANCES_HPP
#define CATEGORYCHANCES_HPP

#include <random>
#include <map>
#include <deque>
#include <stdexcept>
#include <numeric>
#include <string>
#include <cmath>
#include <format>
#include <memory>
#include "Enumeration.hpp"

using std::map;
using std::random_device;
using std::mt19937;
using std::uniform_int_distribution;
using std::uniform_real_distribution;
using std::invalid_argument;
using std::gcd;
using std::deque;
using std::string;
using std::ceil;
using std::format;
using std::unique_ptr;
using std::make_unique;

class Fraction
{
private:
	// The number of allowed digits for precision.
	// e.g. (235.XXXX) -> where X is MaxDigit, so 4 in this case
	static unsigned long MaxDigit;

	// The numerator of the fraction
	unsigned long num;
	
	// The denominator of the fraction
	unsigned long denom;

	void Simplify();

	void Reduce(const int howMuchToRemove);

	unsigned long Digits();

public:
	Fraction(const unsigned long num);

	Fraction(const unsigned long num, const unsigned long denom);

	double Value() const;

	unsigned long _Num() const;

	unsigned long _Denom() const;

	Fraction operator+(const Fraction& f);

	Fraction operator-(const Fraction& f);

	Fraction operator*(const Fraction& f);

	Fraction operator/(const Fraction& f);

	void Print();
};

class CategoryChance
{
private:
	// The decrease factor used when a category is selected
	// 0.6 or 60% in this case
	const Fraction DECREASE = Fraction(3, 5);

	// The number of questions required for the game
	const int QUESTIONS = 15;

	// The enumeration used for iteration
	Enumeration categories;

	// The categories and their tracked chances in every iteration
	map<string, deque<unique_ptr<Fraction>>> categoryChances;

	// Expensive non-deterministic seed for random numbers
	static random_device randomGenerator;

public:
	CategoryChance(const Enumeration& enums);

	deque<string> GenerateRandomizedCategories();

	void RecalculatePercantages(const deque<string>& result);

	string ChosenCategory();

	static int Random(const int lowerBound, const int upperBound);

	static double Random(const double lowerBound, const double upperBound);
};

#endif
