#pragma once

#include <functional>

#if _DEBUG
	#include "String.h"
#endif

// Class Definition ///////////////////////////////////////////////////////

class HashString
{
public:

	HashString();
	HashString(size_t hash);
	HashString(const char* string);
	HashString(string string);
	HashString(const HashString& hash);

	bool				IsNull() const;
	size_t				GetHash() const;

#if _DEBUG
	const char* c_str() const
	{
		return m_string.c_str();
	}
#endif

	bool operator==(const HashString other) const
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
	: m_hash(std::hash<string>{}(input))
	, m_string(input)
{
}

inline HashString::HashString(const HashString& hash)
	: m_hash(hash.m_hash)
	, m_string(string(hash.m_string.c_str())) //TODO: make a copy constructor for string instead of being lazy
{

}

inline HashString::HashString(size_t hash)
	: m_hash(hash)
	, m_string("")
{

}

inline HashString::HashString()
{
	m_hash = 0;
}

inline bool HashString::IsNull() const
{
	return m_hash == 0;
}

inline size_t HashString::GetHash() const
{
	return m_hash;
}


namespace std
{
	template<> struct hash<HashString>
	{
		std::size_t operator()(HashString const& s) const noexcept
		{
			return s.GetHash();
		}
	};
}