#pragma once
#include "Flow/Rendering/RenderAPI.h"
#include <Windows.h>
#include <d3d11.h>
#include <wrl.h>
#include <DirectXMath.h>

namespace Flow
{
	class DX11RenderAPI : public RenderAPI
	{
	public:
		virtual void InitialiseDX11API(HWND WindowHandle, int ViewportWidth, int ViewportHeight) override;

		virtual void SetClearColour(float R, float G, float B, float A) override;
		virtual void Clear() override;

		virtual void BeginFrame() override;
		virtual void EndFrame() override;

		virtual void DrawIndexed(int Count) override;

		ID3D11Device* GetDevice();
		ID3D11DeviceContext* GetContext();

		void SetCameraMatrix(DirectX::FXMMATRIX NewMatrix);
		DirectX::XMMATRIX GetCameraMatrix() const;

	private:
		Microsoft::WRL::ComPtr<IDXGISwapChain> SwapChain = nullptr;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> Context = nullptr;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> RenderTarget = nullptr;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> DepthStencilView = nullptr;
		Microsoft::WRL::ComPtr<ID3D11Device> Device = nullptr;

		float BackgroundColour[4] = { 0.2f, 0.2f, 0.2f, 0.2f };

		DirectX::XMMATRIX CameraMatrix;
	};
}