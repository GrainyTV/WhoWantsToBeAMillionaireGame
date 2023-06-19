#include "CategoryChance.hpp"

random_device CategoryChance::randomGenerator = random_device();

unsigned long Fraction::MaxDigit = 4;

/**
 * 
 * Factorization-based simplification for fractions.
 * 
 */
void Fraction::Simplify()
{
	int commonDivider = gcd(num, denom);
	num = num / commonDivider;
	denom = denom / commonDivider;
}

/**
 * 
 * Custom round function for really small fractions
 * Mostly just for overflow prevention and to avoid high resource computations
 * @param howMuchToRemove : the number of digits we have in extra above MaxDigit
 * 
 */
void Fraction::Reduce(const int howMuchToRemove)
{
	denom = denom / (howMuchToRemove * 10);
	num = static_cast<unsigned long>(ceil(static_cast<double>(num) / (howMuchToRemove * 10)));
}

/**
 * 
 * Returns the number of digits in the denominator.
 * @return : the number of digits
 * 
 */
unsigned long Fraction::Digits()
{
	int digits = 0;
	int number = denom;

	while(number) 
	{
		number /= 10;
		digits++;
	}
	
	return digits;
}

/**
 * 
 * Constructor for integers
 * 
 */
Fraction::Fraction(const unsigned long num) : num(num), denom(1)
{
}

/**
 * 
 * Constructor for real numbers
 *
 */
Fraction::Fraction(const unsigned long num, const unsigned long denom) : num(num), denom(denom)
{
	Simplify();
	const unsigned long DIGITS = (*this).Digits();

	if(DIGITS > MaxDigit)
	{
		(*this).Reduce(DIGITS - MaxDigit);
	}
}

/**
 * 
 * Converts from fraction to double.
 * It is called only when checking the results.
 * @return : the value of the fraction in double
 * 
 */
double Fraction::Value() const
{
	return static_cast<double>(num) / static_cast<double>(denom);
}

/**
 * 
 * Getter function for the numerator.
 * @return : the numerator
 * 
 */
unsigned long Fraction::_Num() const
{
	return num;
}

/**
 * 
 * Getter function for the denominator.
 * @return : the denominator
 * 
 */
unsigned long Fraction::_Denom() const
{
	return denom;
}

/**
 * 
 * Overriden ADD operation.
 * Simplifies and reduces the fraction as well.
 * @param f : the second operand of the operation
 * @return : the result of the operation (copied)
 * 
 */
Fraction Fraction::operator+(const Fraction& f)
{
	Fraction fract(num * f._Denom() + f._Num() * denom, denom * f._Denom());

	fract.Simplify();
	const unsigned long DIGITS = fract.Digits();

	if(DIGITS > MaxDigit)
	{
		fract.Reduce(DIGITS - MaxDigit);
	}

	return fract;
}

/**
 * 
 * Overriden SUBTRACT operation.
 * Simplifies and reduces the fraction as well.
 * @param f : the second operand of the operation
 * @return : the result of the operation (copied)
 * 
 */
Fraction Fraction::operator-(const Fraction& f)
{
	Fraction fract(num * f._Denom() - f._Num() * denom, denom * f._Denom());

	fract.Simplify();
	const unsigned long DIGITS = fract.Digits();

	if(DIGITS > MaxDigit)
	{
		fract.Reduce(DIGITS - MaxDigit);
	}

	return fract;
}

/**
 * 
 * Overriden MULTIPLICATION operation.
 * Simplifies and reduces the fraction as well.
 * @param f : the second operand of the operation
 * @return : the result of the operation (copied)
 * 
 */
Fraction Fraction::operator*(const Fraction& f)
{
	Fraction fract(num * f._Num(), denom * f._Denom());

	fract.Simplify();
	const unsigned long DIGITS = fract.Digits();

	if(DIGITS > MaxDigit)
	{
		fract.Reduce(DIGITS - MaxDigit);
	}

	return fract;
}

/**
 * 
 * Overriden DIVISION operation.
 * Simplifies and reduces the fraction as well.
 * @param f : the second operand of the operation
 * @return : the result of the operation (copied)
 * 
 */
Fraction Fraction::operator/(const Fraction& f)
{
	Fraction fract(num * f._Denom(), denom * f._Num());

	fract.Simplify();
	const unsigned long DIGITS = fract.Digits();

	if(DIGITS > MaxDigit)
	{
		fract.Reduce(DIGITS - MaxDigit);
	}

	return fract;
}

/**
 * 
 * Print function for fractions. Only for debugging purposes.
 * 
 */
void Fraction::Print()
{
	printf("[%lu/%lu] ", num, denom);
}

/**
 * 
 * The constructor sets the default chance for each category. It is calculated as (1 / Total).
 * e.g. We have 10 categories, then it is 1/10.
 * @param enums : our enum values that define a unique category
 * 
 */
CategoryChance::CategoryChance(const Enumeration& enums) : categories(enums)
{
	const int COUNT = categories.Count();

	if(COUNT > 100)
	{
		throw invalid_argument("Cannot handle more than 100 registered categories.");
	}

	for(auto enumEntry : categories)
	{	
		categoryChances[enumEntry.name].push_front(make_unique<Fraction>(1, COUNT));
	}
}

/**
 * 
 * This function iterates over the generation process, the number of required times.
 * So in our case 15 times. We need 15 selected categories to choose the questions from.
 * @result : the name of the chosen categories
 * 
 */
deque<string> CategoryChance::GenerateRandomizedCategories()
{
	deque<string> result;

	for(int i = 0; i < QUESTIONS; ++i)
	{
		RecalculatePercantages(result);
		result.push_back(ChosenCategory());
	}

	return result;
}

/**
 * 
 * The core function of this class. Recalculates the percantages based on the prior selection.
 * @param result: our previously selected category values
 * 
 */
void CategoryChance::RecalculatePercantages(const deque<string>& result)
{
	// We return the first time.
	// Cannot calculate new percantages without prior selection.
	if(result.empty())
	{
		return;
	}

	// A value to check string equality
	const int MATCH = 0;

	// The name of the selected category in the previous iteration
	const string PREVIOUSLY_CHOSEN = result.back();

	// The chance of selected category in the previous iteration
	Fraction PREVIOUS_CHANCE = (*categoryChances[PREVIOUSLY_CHOSEN].front());

	// Using the decrease factor on the previously selected value
	// Formula: X - ( X * Decrease Factor), where X is the previously selected chance
	Fraction NEW_CHANCE = PREVIOUS_CHANCE - (PREVIOUS_CHANCE * DECREASE);

	// Now the equation does not equal to 1. We need to know how much to add to the rest of the items.
	// Formula: ( X - Y ) / number of categories - 1, where X is the previously selected chance and Y is the new one
	Fraction REMAINDER = (PREVIOUS_CHANCE - NEW_CHANCE) / Fraction(categoryChances.size() - 1);
	
	// Saving the new chance value
	categoryChances[PREVIOUSLY_CHOSEN].push_front(make_unique<Fraction>(NEW_CHANCE._Num(), NEW_CHANCE._Denom()));

	// Adding the remainder to the rest of the items, so the equation becomes 1 again.
	for(auto enumEntry : categories)
	{
		const string NAME = enumEntry.name;

		// We skip the item selected in the previous iteration
		if(NAME.compare(PREVIOUSLY_CHOSEN) == MATCH)
		{
			continue;
		}

		Fraction MODIFIED = (*categoryChances[NAME].front()) + REMAINDER;
		categoryChances[NAME].push_front(make_unique<Fraction>(MODIFIED._Num(), MODIFIED._Denom()));
	}
}

/**
 * 
 * This method selects a category based on the randomly generated value and our calculated chances.
 * @return : the chosen category
 * 
 */
string CategoryChance::ChosenCategory()
{
	double chosenRandom = Random(0.0, 1.0);
	Fraction total(0);

	for(auto enumEntry : categories)
	{	
		total = total + (*categoryChances[enumEntry.name].front());

		if(total.Value() >= chosenRandom)
		{
			return enumEntry.name;
		}
	}

	throw invalid_argument(format("Could not select category even with 100 percant chance. Final value: {0:f}, selected random: {1:f}.", total.Value(), chosenRandom));
}

/**
 * 
 * Non-deterministic seeded random number generator for integers.
 * @param lowerBound : inclusive, integer lower bound
 * @param upperBound : inclusive, integer upper bound
 * @return : the chosen integer from the interval
 * 
 */
int CategoryChance::Random(const int lowerBound, const int upperBound)
{
	// seeding the engine:
	mt19937 engine{ randomGenerator() }; 
	uniform_int_distribution<int> distribution{ lowerBound, upperBound };

	// get random numbers with:
	return distribution(engine);
}

/**
 * 
 * Non-deterministic seeded random number generator for real numbers.
 * @param lowerBound : inclusive, double lower bound
 * @param upperBound : inclusive, double upper bound
 * @return : the chosen real number from the interval
 * 
 */
double CategoryChance::Random(const double lowerBound, const double upperBound)
{
	// seeding the engine:
	mt19937 engine{ randomGenerator() }; 
	uniform_real_distribution<double> distribution{ lowerBound, upperBound };

	// get random numbers with:
	return distribution(engine);
}