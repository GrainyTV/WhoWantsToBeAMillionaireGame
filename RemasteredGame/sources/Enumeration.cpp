#include "Enumeration.hpp"

/**
 * 
 * Custom equality operator for enums. Used to check for duplicate entries.
 * @param input : the item to check for equality
 * @return : wether they are equal or not
 * 
 */
bool EnumField::operator==(const EnumField& input) const
{
	const int MATCH = 0;

	if(name.compare(input.name) == MATCH && value == input.value)
	{
		return true;
	}

	return false;
}

/**
 * 
 * A hash function for using this enum class as unordered map keys.
 * @param e : one of the enum entries
 * @return : the unique hash value for that entry
 * 
 */
unsigned int EnumField::Hash::operator()(const EnumField& e) const noexcept
{
	unsigned int hash_1 = hash<string>{}(e.name);
	unsigned int hash_2 = hash<unsigned long int>{}(e.value);
	
	return hash_1 ^ (hash_2 << 1);
}

/**
 * 
 * A basic contains method to check if an item is inside our enum.
 * @param name : the name to check for
 * @return : either found or not
 * 
 */
bool Enumeration::Contains(const string& name) const
{
	const int MATCH = 0;
	
	for(int i = 0; i < enums.size(); ++i)
	{
		if(name.compare(enums[i].name) == MATCH)
		{
			return true;
		}
	}
	
	return false;
}

/**
 * 
 * A basic find method to search for an enum entry's integer value.
 * @param name : the name to check for
 * @return : either an integer or an exception if it is not found
 * 
 */
long unsigned int Enumeration::Find(const string& name) const
{
	const int MATCH = 0;
	
	for(int i = 0; i < enums.size(); ++i)
	{
		if(name.compare(enums[i].name) == MATCH)
		{
			return i;
		}
	}
	
	throw invalid_argument(format("Index of element ({0:s}) cannot be found inside the enum.", name));
}

/**
 * 
 * Default constructor for initializing an empty enum.
 * 
 */
Enumeration::Enumeration()
{
}

/**
 * 
 * Other constructor for initializing an enum from a list of items.
 * @param entries : a list of strings used for entries
 * 
 */
Enumeration::Enumeration(initializer_list<string> entries)
{
	for(auto entry : entries)
	{
		Add(entry);
	}
}

/**
 * 
 * Returns the number of elements inside the enum.
 * @return : the amount
 * 
 */
long unsigned int Enumeration::Count() const 
{
	return enums.size();
}

/**
 * 
 * To add a new element to the enum.
 * @param name : the name of the new element (has to be unique)
 * 
 */
void Enumeration::Add(const string& name)
{
	if(Contains(name))
	{
		throw invalid_argument(format("An entry with the given name ({0:s}) already exists in the enum.", name));
	}
	
	enums.push_back((EnumField) { name, enums.size() });
}

/**
 * 
 * To remove an element from the enum given by name.
 * @param value : the name of an element
 * 
 */
void Enumeration::Remove(const string& name)
{
	if(enums.empty() || Contains(name) == false)
	{
		throw invalid_argument(format("Cannot find entry with given name ({0:s}) for deletion in the enum.", name));
	}
	
	long unsigned int value = Find(name);
	enums.erase(enums.begin() + value);

	for(int i = 0; i < enums.size(); ++i)
	{
		if(enums[i].value != i)
		{
			enums[i].value = i;
		}
	}
}

/**
 * 
 * To remove an element from the enum given by index.
 * @param value : the index of an element
 * 
 */
void Enumeration::Remove(const long unsigned int& value)
{
	if(enums.empty() || value >= enums.size())
	{
		throw invalid_argument(format("Given value ({0:d}) is either equal to or larger than the maximum ({1:d}), or you are trying to delete from empty enum.", value, enums.size()));
	}
	
	enums.erase(enums.begin() + value);

	for(int i = 0; i < enums.size(); ++i)
	{
		if(enums[i].value != i)
		{
			enums[i].value = i;
		}
	}
}

/**
 * 
 * To get an element from the enum given by name.
 * @param name : the name of an element
 * 
 */
EnumField Enumeration::operator[](const string& name) const
{
	if(enums.empty() || Contains(name) == false)
	{
		throw invalid_argument(format("Cannot access entry with given name ({0:s}).", name));
	}
	
	long unsigned int value = Find(name);
	return enums[value];
}

/**
 * 
 * To get an element from the enum given by index.
 * @param value : the index of an element
 * 
 */
EnumField Enumeration::operator[](const long unsigned int& value) const
{
	if(enums.empty() || value >= enums.size())
	{
		throw invalid_argument(format("Given value ({0:d}) is either equal to or larger than the maximum ({1:d}), or you are trying to access element from empty enum.", value, enums.size()));
	}
	
	return enums[value];
}

/**
 * 
 * To get an iterator to the beginning of the enum. (Necessary for foreach loops)
 * @return : the begin iterator
 * 
 */
deque<EnumField>::const_iterator Enumeration::begin() const noexcept
{
	return enums.begin();
}

/**
 * 
 * To get an iterator to the end of the enum. (Necessary for foreach loops)
 * @return : the end iterator
 * 
 */
deque<EnumField>::const_iterator Enumeration::end() const noexcept
{
	return enums.end();
}