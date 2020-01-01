#pragma once

#include <Windows.h>
#include <d3d11.h>
#include <wrl.h>

namespace Flow
{
	class GraphicsDX11
	{
	public:
		GraphicsDX11(HWND WindowHandle, unsigned int Width, unsigned int Height);

		//Remove copy operators
		GraphicsDX11(const GraphicsDX11&) = delete;
		GraphicsDX11& operator=(const GraphicsDX11) = delete;

		void ClearWindow();
		void ClearWindow(float R, float G, float B, float A);

		void EndFrame();

		~GraphicsDX11();

		ID3D11Device* GetDevice();
		ID3D11DeviceContext* GetContext();



	private:
		Microsoft::WRL::ComPtr<IDXGISwapChain> SwapChain = nullptr;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> Context = nullptr;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> RenderTarget = nullptr;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> DepthStencilView = nullptr;
		Microsoft::WRL::ComPtr<ID3D11Device> Device = nullptr;
	};
}
