#pragma once
#include "Flow/Core.h"

#include "RenderCommand.h"
#include "Flow/Rendering/Core/Renderable.h"

namespace Flow
{
	class Renderer
	{
	public:
		static RenderAPI::API GetRenderAPI() { return RenderAPI::GetAPI(); };

		virtual void InitialiseDX11API(HWND WindowHandle, int ViewportWidth, int ViewportHeight) {}

		static void BeginScene();
		static void EndScene();

		static void Submit(Renderable* Renderables);
	};
}