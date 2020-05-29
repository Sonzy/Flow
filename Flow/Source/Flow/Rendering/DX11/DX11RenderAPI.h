#pragma once
#include "Flow/Rendering/RenderAPI.h"
#include <Windows.h>
#include <d3d11.h>
#include <wrl.h>
#include <DirectXMath.h>

#include <dxgidebug.h>

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
	virtual void Draw(unsigned int  Count) override;

	virtual void Resize(int Width, int Height);

	virtual Vector GetScreenToWorldDirection(int X, int Y);

	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetContext();

private:
	Microsoft::WRL::ComPtr<IDXGISwapChain> _SwapChain = nullptr;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> _Context = nullptr;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> _RenderTarget = nullptr;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> _DepthStencilView = nullptr;
	Microsoft::WRL::ComPtr<ID3D11Device> _Device = nullptr;

	/* Debug interfaces */
	//Microsoft::WRL::ComPtr<IDXGIDebug> Debug = nullptr;
	//Microsoft::WRL::ComPtr<ID3D11Debug> Debug2 = nullptr;
};