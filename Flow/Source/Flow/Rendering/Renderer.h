#pragma once
#include "Flow/Core.h"

#include "RenderCommand.h"
#include "Flow/Rendering/Core/Renderable.h"

namespace Flow
{
	class RenderableComponent;

	class FLOW_API Renderer
	{
	public:
		virtual ~Renderer();
		static RenderAPI::API GetRenderAPI() { return RenderAPI::GetAPI(); };

		virtual void InitialiseDX11API(HWND WindowHandle, int ViewportWidth, int ViewportHeight) {}

		static void BeginScene();
		static void EndScene();

		static int SubmitWithoutDraw(Renderable* Renderables);
		static void Submit(Renderable* const Renderables);
		static void Submit(RenderableComponent* const Renderable);

		static void Draw(int Count);

	private:
		static int s_ObjectsRendered;
	};
}