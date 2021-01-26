#pragma once

//= Includes =======================================

#include "Flow/Rendering/RenderAPI.h"

#ifndef NOMINMAX
#define NOMINMAX
#endif // NOMINMAX

#include <Windows.h>
#include <d3d11.h>
#include <wrl.h>
#include <DirectXMath.h>
#include <dxgidebug.h>

//= Class Definition ===============================

class DX11RenderAPI : public RenderAPI
{
public:

	//= Public Functions ============================================

	virtual					~DX11RenderAPI();

	virtual void			InitialiseDX11API(HWND WindowHandle, int ViewportWidth, int ViewportHeight) override;

	virtual void			SetClearColour(float R, float G, float B, float A) override;
	virtual void			Clear() override;

	virtual void			BeginFrame() override;
	virtual void			EndFrame() override;

	virtual void			DrawIndexed(int Count) override;
	virtual void			Draw(unsigned int  Count) override;

	virtual void			Resize(int Width, int Height) override;
	virtual void			ResizeDepthBuffer(int Width, int Height) override;


	virtual void			SetProjectionPerspectiveMatrixDefault() override;
	virtual void			SetProjectionOrthographicMatrixDefault() override;

	virtual API				GetAPI() override { return API::DirectX11; };

	virtual Vector3			GetScreenToWorldDirection(int X, int Y, IntVector2 WindowSize, IntVector2 Origin = IntVector2(0)) override;
	virtual IntVector2      WorldToScreen(Vector3 position) override;

	ID3D11Device*			GetDevice();
	ID3D11DeviceContext*	GetContext();

	virtual void			BindBackBuffer(bool clear = true) override;
	virtual void			BindFrameBuffer(FrameBuffer* Buffer, bool clear = true) override;
	virtual void			BindEditorFrameBuffer(bool clear = true) override;

#if WITH_EDITOR
	void					BindEditorFrameBuffer();
	FrameBuffer*			GetEditorBuffer() const;
#endif

private:

	//= Private Variables ============================================

	Microsoft::WRL::ComPtr<IDXGISwapChain>			m_SwapChain = nullptr;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>		m_Context = nullptr;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>	m_RenderTarget = nullptr;
	Microsoft::WRL::ComPtr<ID3D11Device>			m_Device = nullptr;
	Microsoft::WRL::ComPtr<ID3D11Texture2D>			m_DepthTexture = nullptr;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView>	m_DepthTextureView = nullptr;

	FrameBuffer*									m_CurrentBuffer;

#if WITH_EDITOR
	FrameBuffer*									m_EditorBuffer;
	bool											m_EditorBufferBound;
#endif

	Microsoft::WRL::ComPtr<ID3D11Debug>				m_DeviceDebug = nullptr;
};