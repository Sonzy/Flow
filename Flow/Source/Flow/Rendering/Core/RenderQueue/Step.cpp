#include "Flowpch.h"
#include "Step.h"
#include "Flow/Rendering/Core/Bindable.h"

#include "Flow/Rendering/Core/RenderQueue/RenderQueue.h"
#include "Flow/Rendering/Core/TechniqueProbe.h"

Step::Step(size_t TargetPass)
	: _TargetPass(TargetPass)
{
}

void Step::Submit(const Renderable& Drawable) const
{
	RenderQueue::AcceptJob( Job{this, &Drawable}, _TargetPass);
}

void Step::Bind() const
{
	for (const auto& Bind : _Bindables)
	{
		Bind->Bind();
	}
}

void Step::InitialiseParentReferences(const Renderable& Drawable)
{
	for (auto& Bind : _Bindables)
	{
		Bind->InitialiseParentReferences( Drawable );
	}
}

void Step::AddBindable(std::shared_ptr<Bindable> NewBind)
{
	_Bindables.push_back(std::move(NewBind));
}

void Step::AddBindables(std::vector<std::shared_ptr<Bindable>> Binds)
{
	//TODO: Ensure this is bug free
	_Bindables.insert(_Bindables.end(), Binds.begin(), Binds.end());
}

void Step::AcceptProbe(TechniqueProbe& Probe)
{
	Probe.SetStep(this);

	for (auto& B : _Bindables)
	{
		B->AcceptProbe(Probe);
	}
}
