#include "Flowpch.h"
#include "Job.h"
#include "Flow/Rendering/Core/Renderable.h"
#include "Flow/Rendering/Core/RenderQueue/Step.h"

Job::Job(const Step* NewStep, const Renderable* Drawable)
	: _Step(NewStep), _Renderable(Drawable)
{
}

void Job::Execute() const
{
	_Renderable->Bind();
	_Step->Bind();

	RenderCommand::DrawIndexed(_Renderable->GetIndexCount());
}
