#pragma once
#include "Flow/Rendering/RenderAPI.h"
#include <Windows.h>
#include <d3d11.h>
#include <wrl.h>
#include <DirectXMath.h>
#include "Flow/Rendering/Core/Camera/Camera.h"

#include <dxgidebug.h>

namespace Flow
{
	class DX11RenderAPI : public RenderAPI
	{
	public:
		virtual ~DX11RenderAPI();

		virtual void InitialiseDX11API(HWND WindowHandle, int ViewportWidth, int ViewportHeight) override;

		virtual void SetClearColour(float R, float G, float B, float A) override;
		virtual void Clear() override;

		virtual void BeginFrame() override;
		virtual void EndFrame() override;

		virtual void DrawIndexed(int Count) override;

		virtual void Resize(int Width, int Height);

		virtual Vector GetScreenToWorldDirection(int X, int Y);

		ID3D11Device* GetDevice();
		ID3D11DeviceContext* GetContext();

	private:
		Microsoft::WRL::ComPtr<IDXGISwapChain> SwapChain = nullptr;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> Context = nullptr;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> RenderTarget = nullptr;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> DepthStencilView = nullptr;
		Microsoft::WRL::ComPtr<ID3D11Device> Device = nullptr;

		/* Debug interfaces */
		//Microsoft::WRL::ComPtr<IDXGIDebug> Debug = nullptr;
		//Microsoft::WRL::ComPtr<ID3D11Debug> Debug2 = nullptr;

		float BackgroundColour[4] = { 0.2f, 0.2f, 0.2f, 0.2f };

		//Camera MainCamera_;
	};
}