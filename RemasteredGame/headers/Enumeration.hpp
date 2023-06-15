#ifndef ENUMERATION_HPP
#define ENUMERATION_HPP

#include <deque>
#include <string>
#include <stdexcept>
#include <initializer_list>
#include <format>

using std::deque;
using std::string;
using std::to_string;
using std::invalid_argument;
using std::initializer_list;
using std::format;

class Enumeration
{
private:
	struct EnumField
	{
		string name;
		long unsigned int value;
	};

	deque<EnumField> enums;

	bool Contains(const string& name) const;

	long unsigned int Find(const string& name) const;

public:
	Enumeration();

	Enumeration(initializer_list<string> entries);

	long unsigned int Count() const;

	void Add(const string& name);

	void Remove(const string& name);

	void Remove(const long unsigned int& value);

	EnumField operator[](const string& name) const;

	EnumField operator[](const long unsigned int& value) const;
};

#endif
