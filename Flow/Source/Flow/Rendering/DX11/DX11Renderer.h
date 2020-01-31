#pragma once
#include <Windows.h>
#include <d3d11.h>
#include <wrl.h>
#include "Flow/Rendering/Renderer.h"

namespace Flow
{
	class DX11Renderer : public Renderer
	{
	public:
		DX11Renderer(HWND WindowHandle, int ViewportWidth, int ViewportHeight);

		virtual void ClearWindow(float R, float G, float B, float A) override;
		virtual void EndFrame() override;

		ID3D11DeviceContext* GetContext();
		ID3D11Device* GetDevice();

	private:
		Microsoft::WRL::ComPtr<IDXGISwapChain> SwapChain = nullptr;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> Context = nullptr;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> RenderTarget = nullptr;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> DepthStencilView = nullptr;
		Microsoft::WRL::ComPtr<ID3D11Device> Device = nullptr;
	};
}