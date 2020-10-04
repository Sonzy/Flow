//= Includes =======================================

#include "Flowpch.h"
#include "Job.h"
#include "Flow/Rendering/Core/Renderable.h"
#include "Flow/Rendering/Core/RenderQueue/Step.h"

//= Class Definition - Job =========================

Job::Job(const Step* NewStep, const Renderable* Drawable)
	: m_Step(NewStep)
	, m_Renderable(Drawable)
{
}

void Job::Execute() const
{
	m_Renderable->Bind();
	m_Step->Bind();

	RenderCommand::DrawIndexed(m_Renderable->GetIndexCount());
}
