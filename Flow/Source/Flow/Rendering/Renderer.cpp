#include "Flowpch.h"
#include "Renderer.h"


namespace Flow
{
	void Renderer::BeginScene()
	{
	}

	void Renderer::EndScene()
	{
	}

	void Renderer::Submit(Renderable* const  Renderables)
	{
		CHECK_RETURN(!Renderables, "Renderer::Submit: Renderable was nullptr");
		Renderables->BindAll();
		RenderCommand::DrawIndexed(Renderables->GetIndexBuffer().GetCount());
	}

}