#pragma once
#include "RenderAPI.h"

//DX11 includes
#include <d3d11.h>
#include <DirectXMath.h>

namespace Flow
{
	class FLOW_API RenderCommand
	{
	public:
		
		//= DirectX 11 ===============

		static void InitialiseDX11(HWND WindowHandle, int ViewportWidth, int ViewportHeight);
		static ID3D11Device* DX11GetDevice();
		static ID3D11DeviceContext* DX11GetContext();

		static Camera& GetCamera();


		static void SetClearColour(float R, float G, float B, float A);

		static void DrawIndexed(int Count);
		static void ClearWindow();

		static void BeginFrame();
		static void EndFrame();

	private:
		static RenderAPI* s_RendererAPI;
	};
}