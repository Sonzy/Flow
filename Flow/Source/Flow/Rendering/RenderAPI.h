#pragma once

//= Includes ==================================

#include "Rendering/Core/Camera/Camera.h"
#include "Maths/IntVector2.h"

//= Forward Declarations ======================

class FrameBuffer;

//= Class Definitions =========================

class FLOW_API RenderAPI
{
public:

	//= Public Enums ============================================

	enum class API
	{
		None,
		DirectX11,
		DirectX12,
		Vulkan
	};

public:

	//= Public Functions ========================================

	virtual							~RenderAPI() {};

	virtual void					InitialiseDX11API(HWND WindowHandle, int ViewportWidth, int ViewportHeight);

	virtual void					SetClearColour(float R, float G, float B, float A) = 0;
	virtual void					Clear() = 0;

	virtual void					DrawIndexed(int Count) = 0;
	virtual void					Draw(unsigned int Count) = 0;

	virtual void					BeginFrame() = 0;
	virtual void					EndFrame() = 0;

	virtual void					Resize(int Width, int Height) = 0;
	virtual void					ResizeDepthBuffer(int Width, int Height) = 0;

	virtual void					SetProjectionPerspectiveMatrixDefault() = 0;
	virtual void					SetProjectionOrthographicMatrixDefault() = 0;

	virtual Vector3					GetScreenToWorldDirection(int X, int Y, IntVector2 WindowSize, IntVector2 Origin = IntVector2(0)) = 0;
	virtual IntVector2				WorldToScreen(Vector3 position) = 0;

	virtual API						GetAPI() = 0;

	void							SetMainCamera(CameraBase* Camera) { m_MainCamera = Camera;	}
	CameraBase*						GetMainCamera() const { return m_MainCamera; }

	virtual void					BindBackBuffer(bool clear = true) = 0;
	virtual void					BindFrameBuffer(FrameBuffer* Buffer, bool clear = true) = 0;
	virtual void					BindEditorFrameBuffer(bool clear = true) = 0;

	IntVector2						GetWindowSize() { return m_ViewportSize; }

	void							SetWindowMinimized(bool Minimized)	{ m_WindowMinimised = Minimized; }
	bool							IsWindowMinimized() const			{ return m_WindowMinimised; }

	void							SetNearPlane(float plane);
	void							SetFarPlane(float plane);
	float&							GetNearPlaneRef();
	float&							GetFarPlaneRef();


protected:

	//= Protected Variables ===================================

	float							m_BackgroundColour[4] = { 0.2f, 0.2f, 0.2f, 0.2f };
	CameraBase*						m_MainCamera;
									
	IntVector2						m_ViewportSize;
	IntVector2						m_AdjustedViewportSize;
									
	float							m_NearPlane;
	float							m_FarPlane;

	bool							m_WindowMinimised;
};