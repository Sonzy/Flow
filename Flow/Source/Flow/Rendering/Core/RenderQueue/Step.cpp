//= Includes =========================================

#include "Flowpch.h"
#include "Step.h"
#include "Rendering/Core/Bindable.h"

#include "Rendering/Core/RenderQueue/RenderQueue.h"
#include "Rendering/Core/TechniqueProbe.h"

//= Class Definition - Step ==========================

Step::Step(RenderPass TargetPass)
	: m_TargetPass(TargetPass)
{
}

void Step::Submit(const Renderable& Drawable) const
{
	RenderQueue::AcceptJob( Job{this, &Drawable}, m_TargetPass);
}

void Step::Bind() const
{
	for (const auto& Bind : m_Bindables)
	{
		Bind->Bind();
	}
}

void Step::InitialiseParentReferences(const Renderable& Drawable)
{
	for (auto& Bind : m_Bindables)
	{
		Bind->InitialiseParentReferences( Drawable );
	}
}

void Step::AddBindable(Bindable* NewBind)
{
	m_Bindables.push_back(std::move(NewBind));
}

void Step::AddBindables(std::vector<Bindable*> Binds)
{
	//TODO: Ensure this is bug free
	m_Bindables.insert(m_Bindables.end(), Binds.begin(), Binds.end());
}

void Step::AcceptProbe(TechniqueProbe& Probe)
{
	Probe.SetStep(this);

	for (auto& B : m_Bindables)
	{
		B->AcceptProbe(Probe);
	}
}
