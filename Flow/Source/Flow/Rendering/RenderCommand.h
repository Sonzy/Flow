#pragma once
#include "RenderAPI.h"

//DX11 includes
#include <d3d11.h>
#include <DirectXMath.h>

#include "Flow/Rendering/Core/RenderQueue/Job.h"

class CameraBase;

class FLOW_API RenderCommand
{
public:

	//= DirectX 11 ===============

	static void InitialiseDX11(HWND WindowHandle, int ViewportWidth, int ViewportHeight);
	static ID3D11Device* DX11GetDevice();
	static ID3D11DeviceContext* DX11GetContext();

	//= General ===================

	static std::shared_ptr<CameraBase> GetMainCamera();
	static void SetMainCamera(std::shared_ptr<CameraBase> NewCamera);

	static void Resize(int Width, int Height);
	static void SetClearColour(float R, float G, float B, float A);

	static void DrawIndexed(int Count);
	static void Draw(unsigned int Count);
	static void ClearWindow();

	static void BeginFrame();
	static void EndFrame();

	static void Shutdown();

	static IntVector2D GetWindowSize();

	//= Helper ===============

	static Vector GetScreenToWorldDirectionVector(int X, int Y);

private:
	static RenderAPI* s_RendererAPI;
};
