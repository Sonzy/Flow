#pragma once

// Includes ////////////////////////////////////////////////////////

#ifndef NOMINMAX
	#define NOMINMAX
#endif // NOMINMAX

#include <d3d11.h>
#include <DirectXMath.h>
#include <dxgidebug.h>
#include <Windows.h>
#include "Framework/Types/ComPtr.h"

#include "Maths/IntVector2.h"

class FrameBuffer;

// Class Definition ////////////////////////////////////////////////////////

class DX11RenderAPI
{
public:

	// Public Functions ////////////////////////////////////////////////////////

							~DX11RenderAPI();

	void					Initialise(HWND WindowHandle, int ViewportWidth, int ViewportHeight);

	void					SetClearColour(float R, float G, float B, float A);
	void					Clear();

	void					BeginFrame();
	void					EndFrame();

	void					DrawIndexed(int Count);
	void					Draw(unsigned int  Count);

	void					Resize(int Width, int Height);
	void					ResizeDepthBuffer(int Width, int Height);


	void					SetProjectionPerspectiveMatrixDefault();
	void					SetProjectionOrthographicMatrixDefault();
	void					SetNearPlane(float nearPlane);
	void					SetFarPlane(float farPlane);
	float&					GetNearPlaneRef();
	float&					GetFarPlaneRef();

	IntVector2				GetWindowSize();
	void					SetWindowMinimized(bool minimized);
	bool					IsWindowMinimized();

	Vector3					GetScreenToWorldDirection(int X, int Y, IntVector2 WindowSize, IntVector2 Origin = IntVector2(0));
	IntVector2				WorldToScreen(Vector3 position);

	ID3D11Device*			GetDevice();
	ID3D11DeviceContext*	GetContext();

	void					BindBackBuffer(bool clear = true);
	void					BindFrameBuffer(FrameBuffer* Buffer, bool clear = true);
	void					BindEditorFrameBuffer(bool clear = true);
	void					BindGameFrameBuffer(bool clear = true);

#if WITH_EDITOR
	void					BindEditorFrameBuffer();
	FrameBuffer*			GetEditorBuffer() const;
#endif
	FrameBuffer*			GetGameBuffer() const;

private:

	//=Private Variables ////////////////////////////////////////////////////////

	ComPtr<IDXGISwapChain>			m_swapChain = nullptr;
	ComPtr<ID3D11DeviceContext>		m_context = nullptr;
	ComPtr<ID3D11RenderTargetView>	m_renderTarget = nullptr;
	ComPtr<ID3D11Device>			m_device = nullptr;
	ComPtr<ID3D11Texture2D>			m_depthTexture = nullptr;
	ComPtr<ID3D11DepthStencilView>	m_depthTextureView = nullptr;
	ComPtr<ID3D11Debug>				m_deviceDebug = nullptr;

	FrameBuffer*				m_currentBuffer;

	//Render the game to a seperate buffer if we're in the editor
#if WITH_EDITOR
	FrameBuffer*				m_gameBuffer;
#endif

	float						m_backgroundColour[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	IntVector2					m_viewportSize;
	IntVector2					m_adjustedViewportSize;
	float						m_nearPlane;
	float						m_farPlane;
	bool						m_windowMinimised;
};

// Inline Function Definitions ////////////////////////////////////////////////////////

inline void DX11RenderAPI::SetNearPlane(float nearPlane)
{
	m_nearPlane = nearPlane;
}

inline void DX11RenderAPI::SetFarPlane(float farPlane)
{
	m_farPlane = farPlane;
}

inline float& DX11RenderAPI::GetNearPlaneRef()
{
	return m_nearPlane;
}

inline float& DX11RenderAPI::GetFarPlaneRef()
{
	return m_farPlane;
}

inline IntVector2 DX11RenderAPI::GetWindowSize()
{
	return m_viewportSize;
}

inline void DX11RenderAPI::SetWindowMinimized(bool minimized)
{
	m_windowMinimised = minimized;
}

inline bool DX11RenderAPI::IsWindowMinimized()
{
	return m_windowMinimised;
}
