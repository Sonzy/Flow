#include "Flowpch.h"
#include "Renderer.h"
#include "Flow\GameFramework\Components\RenderableComponent.h"
#include "Flow/Rendering/Core/RenderQueue/Technique.h"
#include "Flow/Rendering/Core/RenderQueue/RenderQueue.h"

int Renderer::s_ObjectsRendered = 0;
Renderer::~Renderer()
{
}
void Renderer::BeginScene()
{
	s_ObjectsRendered = 0;
}

void Renderer::EndScene()
{
	RenderQueue::Execute();
	RenderQueue::Reset();
	//FLOW_ENGINE_LOG("Rendered: {0} objects.", s_ObjectsRendered);
}

int Renderer::SubmitWithoutDraw(Renderable* Renderables)
{
	FLOW_ENGINE_WARNING("Renderer::SubmitWithoutDraw: TODO: Update");
	//CHECK_RETURN_ZERO(!Renderables, "Renderer::Submit: Renderable was nullptr");
	//
	//Renderables->BindAll();
	//s_ObjectsRendered++;
	//return Renderables->GetIndexBuffer().GetCount();
	return 0;
}

void Renderer::Submit(Renderable* const  Object)
{
	PROFILE_FUNCTION();

	CHECK_RETURN(!Object, "Renderer::Submit: Renderable was nullptr");

	for (const auto& T : Object->_Techniques)
	{
		T.Submit(*Object);
	}

	//Renderables->BindAll();
	//RenderCommand::DrawIndexed(Renderables->GetIndexBuffer().GetCount());

	s_ObjectsRendered++;
}

void Renderer::Draw(int Count)
{
	RenderCommand::DrawIndexed(Count);
}