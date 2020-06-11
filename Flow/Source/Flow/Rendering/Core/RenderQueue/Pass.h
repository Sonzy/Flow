#pragma once
#include <vector>
#include "Flow/Rendering/Core/RenderQueue/Job.h"

class Pass
{
public:

	void Accept(Job NewJob);
	void Execute() const;
	void Reset();

private:
	std::vector<Job> _Jobs;
};