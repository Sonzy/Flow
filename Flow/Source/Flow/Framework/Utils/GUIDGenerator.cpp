// Includes ////////////////////////////////////////////////////////////

#include "pch.h"
#include "GUIDGenerator.h"
#include <random>

// Static Variable Definitions /////////////////////////////////////////

std::random_device						GUIDGen::m_RandomDevice;
std::mt19937							GUIDGen::m_MTEngine(GUIDGen::m_RandomDevice());
std::uniform_int_distribution<FGuid>	GUIDGen::m_Distribution;

// Function Definitions ////////////////////////////////////////////////

FGuid GUIDGen::Generate()
{
	//TODO: Better guid gen 
	//Generate a random 32 bit number.
	return m_Distribution(m_MTEngine);
}