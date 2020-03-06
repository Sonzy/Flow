#include "Flowpch.h"
#include "Renderer.h"
namespace Flow
{
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
		//FLOW_ENGINE_LOG("Rendered: {0} objects.", s_ObjectsRendered);
	}

	int Renderer::SubmitWithoutDraw(Renderable* Renderables)
	{
		CHECK_RETURN_ZERO(!Renderables, "Renderer::Submit: Renderable was nullptr");

		Renderables->BindAll();
		s_ObjectsRendered++;
		return Renderables->GetIndexBuffer().GetCount();
	}

	void Renderer::Submit(Renderable* const  Renderables)
	{
		CHECK_RETURN(!Renderables, "Renderer::Submit: Renderable was nullptr");

		Renderables->BindAll();
		RenderCommand::DrawIndexed(Renderables->GetIndexBuffer().GetCount());
		s_ObjectsRendered++;
	}

	void Renderer::Draw(int Count)
	{
		RenderCommand::DrawIndexed(Count);
	}

}