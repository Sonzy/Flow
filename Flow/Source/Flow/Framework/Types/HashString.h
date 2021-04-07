#pragma once

#include <functional>

#if _DEBUG
#include "String.h"
#endif

// Class Definition ///////////////////////////////////////////////////////

class HashString
{
public:

	HashString(const char* string);
	HashString(string string);

	bool				IsNull() const;

#if _DEBUG
	const char* c_str() const
	{
		return m_string.c_str();
	}
#endif

	bool operator==(const HashString other)
	{
		return m_hash == other.m_hash;
	}

private:

	size_t m_hash;
#if _DEBUG
	string m_string;
#endif
};

// Inline Function Definitions ///////////////////////////////////////////////////////

inline HashString::HashString(const char* cstr)
	: HashString(string(cstr))
{

}

inline HashString::HashString(string input)
{
	m_hash = std::hash<string>{}(input);
}

inline bool HashString::IsNull() const
{
	return m_hash == 0;
}