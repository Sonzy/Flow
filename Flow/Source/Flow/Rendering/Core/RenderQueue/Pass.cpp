//= Includes ==========================

#include "Flowpch.h"
#include "Pass.h"

//= Class Definition - Pass ===========

Pass::Pass()
	: m_enabled(true)
{
}

void Pass::Accept(Job NewJob)
{
	m_jobs.push_back(NewJob);
}

void Pass::Execute() const
{
	PROFILE_FUNCTION();

	for (const auto& J : m_jobs)
	{
		J.Execute();
	}
}

void Pass::Reset()
{
	m_jobs.clear();
}

bool Pass::IsEnabled() const
{
	return m_enabled;
}

bool& Pass::Enabled()
{
	return m_enabled;
}

void Pass::SetEnabled(bool enabled)
{
	m_enabled = enabled;
}
