#include "Flowpch.h"
#include "Timer.h"

Timer::Timer()
{
	_Last = std::chrono::steady_clock::now();
}

float Timer::Mark()
{
	const std::chrono::steady_clock::time_point old = _Last;
	_Last = std::chrono::steady_clock::now();
	const std::chrono::duration<float> DeltaTime = _Last - old;
	return DeltaTime.count();
}

float Timer::Peek()
{
	return std::chrono::duration<float>(std::chrono::steady_clock::now() - _Last).count();
}