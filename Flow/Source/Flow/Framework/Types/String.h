#pragma once
#include <Typedefs.h>

// Class Definition ///////////////////////////////////////////////////////

class string
{
	// Public Classes ////////////////////////////////////////////////////

	class Iterator
	{
	public:
		Iterator(const char* ptr)
			: m_ptr(ptr)
		{

		}

	private:
		const char* m_ptr;
	};


public:
	// Public Functions //////////////////////////////////////////////////

	string();
	string(const char* data);
	string(const string& other);

public:

	void append(const string& other)
	{
		other.c_str();
	}

	void append(const char* other)
	{
		uint32 otherLength = static_cast<uint32>(strlen(other));
		uint32 newlength = m_length + otherLength;
		char* newbuffer = new char[newlength];
		memcpy(&newbuffer, m_ptr, m_length);
		memcpy(&newbuffer + otherLength, m_ptr + m_length, otherLength);
		m_ptr = newbuffer;
	}

	//= Iterators =

	Iterator begin()
	{
		return Iterator(m_ptr);
	}

	Iterator end()
	{
		return Iterator(m_ptr + m_length);
	}


	//= STL Functions =

	const char* c_str() const
	{
		return m_ptr;
	}

	uint32 length() const
	{
		return m_length;
	}

public:

	// Operators /////////////////////////////////////////////////////////

	void operator+=(const string& other)
	{
		append(other.c_str());
	}

private:

	char*			m_ptr;
	uint32			m_length;
};

// Inline Functions ///////////////////////////////////////////

inline string::string()
{
	m_ptr = nullptr;
	m_length = 0;
}

inline string::string(const char* data)
{
	if (data == nullptr)
	{
		m_length = 0;
		m_ptr = nullptr;
		return;
	}

	m_length = static_cast<uint32>(strlen(data)) + 1;
	m_ptr = new char[m_length];
	memcpy(m_ptr, data, sizeof(char) * m_length);
}

inline string::string(const string& other)
	: string(other.c_str())
{

}


//STL Hash method for string
//TODO: see what FNV is 
inline constexpr size_t fnv_prime = 1099511628211ULL; 
inline constexpr size_t fnv_offset_basis = 4695981039346656037ULL; 

namespace std
{
	template<> struct hash<::string>
	{
		std::size_t operator()(::string const& s) const noexcept
		{
			if (s.length() == 0)
			{
				return 0;
			}

			const char* first = s.c_str();
			size_t hashedValue = fnv_offset_basis;
			for (size_t i = 0; i < s.length(); ++i)
			{
				hashedValue ^= static_cast<size_t>(first[i]);
				hashedValue *= fnv_prime;
			}

			return hashedValue;
		}
	};
}