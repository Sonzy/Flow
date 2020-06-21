#pragma once
#include "Flow/Rendering/Core/Camera/Camera.h"

class FrameBuffer;

class FLOW_API RenderAPI
{
public:

	enum class API
	{
		None,
		DirectX11,
		DirectX12,
		Vulkan
	};

public:
	virtual ~RenderAPI() {};

	virtual void InitialiseDX11API(HWND WindowHandle, int ViewportWidth, int ViewportHeight);

	virtual void SetClearColour(float R, float G, float B, float A) = 0;
	virtual void Clear() = 0;

	virtual void DrawIndexed(int Count) = 0;
	virtual void Draw(unsigned int Count) = 0;

	virtual void BeginFrame() = 0;
	virtual void EndFrame() = 0;

	virtual void Resize(int Width, int Height) = 0;
	virtual void ResizeDepthBuffer(int Width, int Height) = 0;

	virtual Vector GetScreenToWorldDirection(int X, int Y, IntVector2D WindowSize, IntVector2D Origin = IntVector2D(0)) = 0;

	static API GetAPI() { return s_API; };

	void SetMainCamera(std::shared_ptr<CameraBase> Camera) { _MainCamera = Camera;	}
	std::shared_ptr<CameraBase> GetMainCamera() const { return _MainCamera; }

	virtual void BindBackBuffer() = 0;
	virtual void BindFrameBuffer(FrameBuffer* Buffer) = 0;

public:

	IntVector2D GetWindowSize() { return _ViewportSize; }

private:
	static API s_API;

protected:

	float _BackgroundColour[4] = { 0.2f, 0.2f, 0.2f, 0.2f };
	std::shared_ptr<CameraBase> _MainCamera;

	IntVector2D _ViewportSize;
	IntVector2D _AdjustedViewportSize;

	float _NearPlane;
	float _FarPlane;
};