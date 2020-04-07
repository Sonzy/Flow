#pragma once
#include <chrono>

namespace Flow
{
	class Timer
	{
	public:

		Timer();

		// Returns time since last called
		float Mark();

		// Returns time since last called without resetting timer
		float Peek();

	private:
		std::chrono::steady_clock::time_point Last_;
	};
}