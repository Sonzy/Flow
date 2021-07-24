#pragma once
#include <random>
#include "Typedefs.h"

#define INVALID_GUID -1

class GUIDGen
{
public:

	static FGuid			Generate();

private:

	static std::random_device						m_RandomDevice;
	static std::mt19937								m_MTEngine;
	static std::uniform_int_distribution<FGuid>		m_Distribution;
};