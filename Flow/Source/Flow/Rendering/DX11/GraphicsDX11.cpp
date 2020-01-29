#include "Flowpch.h"
#include "GraphicsDX11.h"
#include "ThirdParty/ImGui/imgui.h"
#include "ThirdParty/ImGui/examples/imgui_impl_dx11.h"
#include "Flow/ErrorHandling/dxerr.h"
#include "Flow/ErrorHandling/ErrorMacros.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")

//TODO: Change resolution on window res change

Flow::GraphicsDX11::GraphicsDX11(HWND WindowHandle, unsigned int Width, unsigned int Height)
{
	HRESULT ResultHandle;

	DXGI_SWAP_CHAIN_DESC SwapchainDesc;
	SwapchainDesc.BufferDesc.Width = Width;
	SwapchainDesc.BufferDesc.Height = Height;
	SwapchainDesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	SwapchainDesc.BufferDesc.RefreshRate.Denominator = 0;
	SwapchainDesc.BufferDesc.RefreshRate.Numerator = 0;
	SwapchainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	SwapchainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	SwapchainDesc.SampleDesc.Count = 1;
	SwapchainDesc.SampleDesc.Quality = 0;
	SwapchainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	SwapchainDesc.BufferCount = 1;
	SwapchainDesc.OutputWindow = WindowHandle;
	SwapchainDesc.Windowed = TRUE;
	SwapchainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	SwapchainDesc.Flags = 0;

	UINT SwapCreateFlags = 0;
	SwapCreateFlags |= D3D11_CREATE_DEVICE_DEBUG; //TODO: Disable in non debug mode

	CATCH_ERROR_DX(
		D3D11CreateDeviceAndSwapChain(
			nullptr,
			D3D_DRIVER_TYPE_HARDWARE,
			nullptr,
			SwapCreateFlags,
			nullptr,
			0,
			D3D11_SDK_VERSION,
			&SwapchainDesc,
			SwapChain.GetAddressOf(),
			Device.GetAddressOf(),
			nullptr,
			Context.GetAddressOf())
	);

	Microsoft::WRL::ComPtr<ID3D11Resource> BackBuffer = nullptr;

	// Create render target view and populate backbuffer
	CATCH_ERROR_DX(SwapChain->GetBuffer(0, __uuidof(ID3D11Resource), &BackBuffer));
	CATCH_ERROR_DX(Device->CreateRenderTargetView(BackBuffer.Get(), nullptr, &RenderTarget));


	// Create Depth Stencil State
	D3D11_DEPTH_STENCIL_DESC DepthStencilDescription = {};
	DepthStencilDescription.DepthEnable = true;
	DepthStencilDescription.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	DepthStencilDescription.DepthFunc = D3D11_COMPARISON_LESS;
	DepthStencilDescription.StencilEnable = FALSE;

	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> DepthStencilState = nullptr;
	CATCH_ERROR_DX(Device->CreateDepthStencilState(&DepthStencilDescription, &DepthStencilState));

	// Bind the Depth State
	Context->OMSetDepthStencilState(DepthStencilState.Get(), 1u);

	// Create the depth stencil texture
	Microsoft::WRL::ComPtr<ID3D11Texture2D> DepthStencil = nullptr;
	D3D11_TEXTURE2D_DESC DepthDescription = {};
	DepthDescription.Width = Width;
	DepthDescription.Height = Height;
	DepthDescription.MipLevels = 1u;
	DepthDescription.ArraySize = 1u;
	DepthDescription.Format = DXGI_FORMAT_D32_FLOAT;
	DepthDescription.SampleDesc.Count = 1u;
	DepthDescription.SampleDesc.Quality = 0u;
	DepthDescription.Usage = D3D11_USAGE_DEFAULT;
	DepthDescription.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	CATCH_ERROR_DX(Device->CreateTexture2D(&DepthDescription, nullptr, &DepthStencil));

	// Create the depth stencil view
	D3D11_DEPTH_STENCIL_VIEW_DESC DepthStencilViewDescription = {};
	DepthStencilViewDescription.Format = DXGI_FORMAT_D32_FLOAT;
	DepthStencilViewDescription.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	DepthStencilViewDescription.Texture2D.MipSlice = 0u;
	CATCH_ERROR_DX(Device->CreateDepthStencilView(DepthStencil.Get(), &DepthStencilViewDescription, &DepthStencilView));

	// Bind the Depth Stencil to the Output Merger
	Context->OMSetRenderTargets(1u, RenderTarget.GetAddressOf(), DepthStencilView.Get());

	//Setup Viewport
	D3D11_VIEWPORT Viewport;
	Viewport.Width = Width;
	Viewport.Height = Height;
	Viewport.MinDepth = 0.0f;
	Viewport.MaxDepth = 1.0f;
	Viewport.TopLeftX = 0.0f;
	Viewport.TopLeftY = 0.0f;

	Context->RSSetViewports(1u, &Viewport);
}

void Flow::GraphicsDX11::ClearWindow()
{
	ClearWindow(0, 0, 0, 1);
}

void Flow::GraphicsDX11::ClearWindow(float R, float G, float B, float A)
{
	const float Colour[4] = { R, G, B, A };
	Context->ClearRenderTargetView(RenderTarget.Get(), Colour);
	Context->ClearDepthStencilView(DepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0u);
}

void Flow::GraphicsDX11::EndFrame()
{
	HRESULT ResultHandle;
	CATCH_ERROR_DX(SwapChain->Present(1, 0));
}

Flow::GraphicsDX11::~GraphicsDX11()
{
}

ID3D11Device* Flow::GraphicsDX11::GetDevice()
{
	return Device.Get();
}

ID3D11DeviceContext* Flow::GraphicsDX11::GetContext()
{
	return Context.Get();
}
