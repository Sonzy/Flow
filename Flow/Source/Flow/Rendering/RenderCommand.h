#pragma once

//= Includes =====================================

#include "RenderAPI.h"

//DX11 includes
#include <d3d11.h>
#include <DirectXMath.h>

#include "Rendering/Core/RenderQueue/Job.h"
#include "Maths/IntVector2.h"

//= Forward Declarations =========================

class CameraBase;
class FrameBuffer;

//= Class Definitions ============================

class FLOW_API RenderCommand
{
public:

	//= Public Static Functions =========================================

	//= DirectX 11 =

	static void							InitialiseDX11(HWND WindowHandle, int ViewportWidth, int ViewportHeight);
	static ID3D11Device*				DX11GetDevice();
	static ID3D11DeviceContext*			DX11GetContext();

	//= General =

	static RenderAPI::API				GetAPI();
	static std::shared_ptr<CameraBase>	GetMainCamera();
	static void							SetMainCamera(std::shared_ptr<CameraBase> NewCamera);

	static void							Resize(int Width, int Height);
	static void							SetClearColour(float R, float G, float B, float A);

	static void							DrawIndexed(int Count);
	static void							Draw(unsigned int Count);
	static void							ClearWindow();

	static void							BeginFrame();
	static void							EndFrame();

	static void							Shutdown();

#if WITH_EDITOR
	static void							BindEditorBuffer();
	static FrameBuffer*					GetEditorFrameBuffer();
#endif
	static void							BindFrameBuffer(FrameBuffer* Buffer);
	static void							BindBackBuffer();

	static IntVector2					GetWindowSize();

	//= Helper =

	static Vector3						GetScreenToWorldDirectionVector(int X, int Y, IntVector2 WindowSize, IntVector2 Origin = IntVector2(0,0));

private:

	//= Private Static Variables =========================================

	static RenderAPI*					sm_RendererAPI;
};
