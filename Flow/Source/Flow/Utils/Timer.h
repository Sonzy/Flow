#pragma once

//= Includes ==================================

#include <chrono>

//= Class Definition ==========================

class Timer
{
public:

	//= Public Functions =============

					Timer();


	float			Mark();	// Returns time since last called
	float			Peek();	// Returns time since last called without resetting timer

private:
	
	//= Private Variables ============
	std::chrono::steady_clock::time_point m_Last;
};