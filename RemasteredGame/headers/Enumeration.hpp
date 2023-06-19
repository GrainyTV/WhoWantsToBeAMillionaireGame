#ifndef ENUMERATION_HPP
#define ENUMERATION_HPP

#include <deque>
#include <string>
#include <stdexcept>
#include <initializer_list>
#include <format>
#include <functional>
#include <iterator>

using std::deque;
using std::string;
using std::to_string;
using std::invalid_argument;
using std::initializer_list;
using std::format;
using std::hash;

struct EnumField
{
	string name;
	
	long unsigned int value;

	bool operator==(const EnumField& input) const;

	struct Hash
	{
		unsigned int operator()(const EnumField& e) const noexcept;
	};
};

class Enumeration
{
private:
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

	deque<EnumField>::const_iterator begin() const noexcept;

	deque<EnumField>::const_iterator end() const noexcept;
};

#endif
