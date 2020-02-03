#include "Flowpch.h"
#include "Timer.h"

namespace Flow
{
	Timer::Timer()
	{
		last = std::chrono::steady_clock::now();
	}

	float Timer::Mark()
	{
		const std::chrono::steady_clock::time_point old = last;
		last = std::chrono::steady_clock::now();
		const std::chrono::duration<float> DeltaTime = last - old;
		return DeltaTime.count();
	}

	float Timer::Peek()
	{
		return std::chrono::duration<float>(std::chrono::steady_clock::now() - last).count();
	}
}
