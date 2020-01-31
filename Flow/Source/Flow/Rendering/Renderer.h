#pragma once
#include "Flow/Core.h"

namespace Flow
{
	enum class RenderAPI
	{
		None,
		DirectX11,
		DirectX12,
		Vulkan
	};


	class FLOW_API Renderer
	{
		static RenderAPI s_RendererAPI;

	public:
		static void SetRenderAPI(RenderAPI API);
		static RenderAPI GetRenderAPI();

	};
}