//= Includes ==================================

#include "pch.h"
#include "Technique.h"
#include "Rendering/Core/RenderQueue/Step.h"
#include "Rendering/Core/TechniqueProbe.h"

//= Class Definition - Technique ==============

Technique::Technique(const std::string& Name)
	: m_Name(Name)
{
}

void Technique::Submit(const Renderable& Drawable) const
{
	if (m_Active)
	{
		for (const auto& Step : m_Steps)
		{
			Step.Submit(Drawable);
		}
	}
}

void Technique::AddStep(Step NewStep)
{
	m_Steps.push_back(std::move(NewStep));
}

void Technique::Activate()
{
	m_Active = true;
}

void Technique::Deactivate()
{
	m_Active = false;
}

void Technique::InitialiseParentReferences(const Renderable& Parent)
{
	for (auto& S : m_Steps)
	{
		S.InitialiseParentReferences(Parent);
	}
}

void Technique::AcceptProbe(TechniqueProbe& Probe)
{
	Probe.SetTechnique(this);

	for (auto& S : m_Steps)
	{
		S.AcceptProbe(Probe);
	}
}
