#include "pch.h"
#include "Timer.h"

Timer::Timer()
{
	m_Last = std::chrono::steady_clock::now();
}

float Timer::Mark()
{
	const std::chrono::steady_clock::time_point old = m_Last;
	m_Last = std::chrono::steady_clock::now();
	const std::chrono::duration<float> DeltaTime = m_Last - old;
	return DeltaTime.count();
}

float Timer::Peek()
{
	return std::chrono::duration<float>(std::chrono::steady_clock::now() - m_Last).count();
}