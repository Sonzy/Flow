//= Includes ==========================

#include "Flowpch.h"
#include "Pass.h"

//= Class Definition - Pass ===========

void Pass::Accept(Job NewJob)
{
	m_Jobs.push_back(NewJob);
}

void Pass::Execute() const
{
	PROFILE_FUNCTION();

	for (const auto& J : m_Jobs)
	{
		J.Execute();
	}
}

void Pass::Reset()
{
	m_Jobs.clear();
}
