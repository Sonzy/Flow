#include "Flowpch.h"
#include "Timer.h"

namespace Flow
{
	Timer::Timer()
	{
		Last_ = std::chrono::steady_clock::now();
	}

	float Timer::Mark()
	{
		const std::chrono::steady_clock::time_point old = Last_;
		Last_ = std::chrono::steady_clock::now();
		const std::chrono::duration<float> DeltaTime = Last_ - old;
		return DeltaTime.count();
	}

	float Timer::Peek()
	{
		return std::chrono::duration<float>(std::chrono::steady_clock::now() - Last_).count();
	}
}
