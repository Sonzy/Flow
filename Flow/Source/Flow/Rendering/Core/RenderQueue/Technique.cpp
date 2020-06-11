#include "Flowpch.h"
#include "Technique.h"
#include "Flow/Rendering/Core/RenderQueue/Step.h"

void Technique::Submit(const Renderable& Drawable) const
{
	if (_Active)
	{
		for (const auto& Step : _Steps)
		{
			Step.Submit(Drawable);
		}
	}
}

void Technique::AddStep(Step NewStep)
{
	_Steps.push_back(std::move(NewStep));
}

void Technique::Activate()
{
	_Active = true;
}

void Technique::Deactivate()
{
	_Active = false;
}

void Technique::InitialiseParentReferences(const Renderable& Parent)
{
	for (auto& S : _Steps)
	{
		S.InitialiseParentReferences(Parent);
	}
}
