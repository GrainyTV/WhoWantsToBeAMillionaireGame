#include "Enumeration.hpp"

bool EnumField::operator==(const EnumField& input) const
{
	const int MATCH = 0;

	if(name.compare(input.name) == MATCH && value == input.value)
	{
		return true;
	}

	return false;
}

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

Enumeration::Enumeration()
{
}

Enumeration::Enumeration(initializer_list<string> entries)
{
	for(auto entry : entries)
	{
		Add(entry);
	}
}

long unsigned int Enumeration::Count() const 
{
	return enums.size();
}

void Enumeration::Add(const string& name)
{
	if(Contains(name))
	{
		throw invalid_argument(format("An entry with the given name ({0:s}) already exists in the enum.", name));
	}
	
	enums.push_back((EnumField) { name, enums.size() });
}

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

EnumField Enumeration::operator[](const string& name) const
{
	if(enums.empty() || Contains(name) == false)
	{
		throw invalid_argument(format("Cannot access entry with given name ({0:s}).", name));
	}
	
	long unsigned int value = Find(name);
	return enums[value];
}

EnumField Enumeration::operator[](const long unsigned int& value) const
{
	if(enums.empty() || value >= enums.size())
	{
		throw invalid_argument(format("Given value ({0:d}) is either equal to or larger than the maximum ({1:d}), or you are trying to access element from empty enum.", value, enums.size()));
	}
	
	return enums[value];
}