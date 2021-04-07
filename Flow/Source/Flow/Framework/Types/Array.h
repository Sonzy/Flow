#pragma once
#include "Typedefs.h"

template <typename T, typename _SizeType = uint32>
class Array
{
public:

	Array()
		: m_capacity(16)
		, m_length(0)
	{
		AllocateBuffer(m_capacity);
	}

	Array(int initialSize)
		: m_capacity(initialSize)
		, m_length(0)
	{
		AllocateBuffer(m_capacity);
	}

	void Add(T& element)
	{
		if (m_length == m_capacity)
		{
			GrowBuffer();
		}

		m_data[m_length] = element;
		m_length++;
	}

	T* Data()
	{
		return m_data;
	}

	const T* Data() const
	{
		return m_data;
	}

	_SizeType Length() const
	{
		return m_length;
	}

private:

	void GrowBuffer()
	{
		T* oldData = m_data;
		AllocateBuffer(m_capacity * 2);
		CopyDataToBuffer(oldData);

		delete oldData;
	}

	void AllocateBuffer(int size)
	{
		m_data = new T[size];
		m_capacity = size;
	}

	void CopyDataToBuffer(T* oldData)
	{
		for (_SizeType i = 0; i < m_length; i++)
		{
			m_data[i] = oldData[i];
		}
	}

private:

	T*					m_data;
	_SizeType			m_length;
	_SizeType			m_capacity;
};

class FixedArray
{

};