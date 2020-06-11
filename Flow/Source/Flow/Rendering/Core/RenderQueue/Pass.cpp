#include "Flowpch.h"
#include "Pass.h"

void Pass::Accept(Job NewJob)
{
	_Jobs.push_back(NewJob);
}

void Pass::Execute() const
{
	PROFILE_FUNCTION();

	for (const auto& J : _Jobs)
	{
		J.Execute();
	}
}

void Pass::Reset()
{
	_Jobs.clear();
}
