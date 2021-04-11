//= Includes =======================================

#include "pch.h"
#include "Job.h"
#include "Rendering/Core/Renderable.h"
#include "Rendering/Core/RenderQueue/Step.h"
#include "Rendering/Renderer.h"

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

	Renderer::DrawIndexed(m_Renderable->GetIndexCount());
}
