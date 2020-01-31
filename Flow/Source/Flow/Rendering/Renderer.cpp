#include "Flowpch.h"
#include "Renderer.h"

namespace Flow
{
	RenderAPI Renderer::s_RendererAPI = RenderAPI::DirectX11;

	void Renderer::SetRenderAPI(RenderAPI API)
	{
		s_RendererAPI = API;
	}

	RenderAPI Renderer::GetRenderAPI()
	{
		return s_RendererAPI;
	}

	void Renderer::ClearWindow()
	{
		ClearWindow(0,0,0,1);
	}

}