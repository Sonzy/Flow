#pragma once
#include <chrono>

class Timer
{
public:

	Timer();

	// Returns time since last called
	float Mark();

	// Returns time since last called without resetting timer
	float Peek();

private:
	std::chrono::steady_clock::time_point _Last;
};