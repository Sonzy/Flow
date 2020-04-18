#include "Flowpch.h"
#include "DX11RenderAPI.h"
#include "ThirdParty/ImGui/imgui.h"
#include "ThirdParty/ImGui/examples/imgui_impl_dx11.h"

#include "Flow\Rendering\RenderCommand.h"
#include "Flow\Application.h"
#include "Flow\Window\WinWindow.h"

#include "Flow\GameFramework\World.h"
#include "Flow\GameFramework\Controllers\PlayerController.h"
#include "Flow\GameFramework\Components\CameraComponent.h"



#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")
#pragma comment(lib, "dxguid.lib")


namespace Flow
{
	DX11RenderAPI::~DX11RenderAPI()
	{
	}

	void DX11RenderAPI::InitialiseDX11API(HWND WindowHandle, int ViewportWidth, int ViewportHeight)
	{
		NearPlane_ = 0.5f;
		FarPlane_ = 2000.0f;
		ViewportSize_ = IntVector2D(ViewportWidth, ViewportHeight);

		HRESULT ResultHandle;

		DXGI_SWAP_CHAIN_DESC SwapchainDesc;
		SwapchainDesc.BufferDesc.Width = ViewportWidth;
		SwapchainDesc.BufferDesc.Height = ViewportHeight;
		SwapchainDesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		SwapchainDesc.BufferDesc.RefreshRate.Denominator = 0;
		SwapchainDesc.BufferDesc.RefreshRate.Numerator = 0;
		SwapchainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		SwapchainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		SwapchainDesc.SampleDesc.Count = 1u;
		SwapchainDesc.SampleDesc.Quality = 0;
		SwapchainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		SwapchainDesc.BufferCount = 2; //flip discard
		SwapchainDesc.OutputWindow = WindowHandle;
		SwapchainDesc.Windowed = TRUE;
		SwapchainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD; //flip discard
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

		// Create the depth stencil texture
		Microsoft::WRL::ComPtr<ID3D11Texture2D> DepthStencil = nullptr;
		D3D11_TEXTURE2D_DESC DepthDescription = {};
		DepthDescription.Width = ViewportWidth;
		DepthDescription.Height = ViewportHeight;
		DepthDescription.MipLevels = 1u;
		DepthDescription.ArraySize = 1u;
		DepthDescription.Format = DXGI_FORMAT_D24_UNORM_S8_UINT; //24 bit depth, 8 bit stencil
		DepthDescription.SampleDesc.Count= 1u;
		DepthDescription.SampleDesc.Quality = 0u;
		DepthDescription.Usage = D3D11_USAGE_DEFAULT;
		DepthDescription.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		CATCH_ERROR_DX(Device->CreateTexture2D(&DepthDescription, nullptr, &DepthStencil));

		// Create the depth stencil view
		D3D11_DEPTH_STENCIL_VIEW_DESC DepthStencilViewDescription = {};
		DepthStencilViewDescription.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		DepthStencilViewDescription.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		DepthStencilViewDescription.Texture2D.MipSlice = 0u;
		CATCH_ERROR_DX(Device->CreateDepthStencilView(DepthStencil.Get(), &DepthStencilViewDescription, &DepthStencilView));

		// Bind the Depth Stencil to the Output Merger
		Context->OMSetRenderTargets(1u, RenderTarget.GetAddressOf(), DepthStencilView.Get());

		//Setup Viewport
		D3D11_VIEWPORT Viewport;
		Viewport.Width = (FLOAT)ViewportWidth;
		Viewport.Height = (FLOAT)ViewportHeight;
		Viewport.MinDepth = 0.0f;
		Viewport.MaxDepth = 1.0f;
		Viewport.TopLeftX = 0.0f;
		Viewport.TopLeftY = 0.0f;

		Context->RSSetViewports(1u, &Viewport);





		//=========================================

		//D3D11_DEPTH_STENCIL_DESC Description = CD3D11_DEPTH_STENCIL_DESC{ CD3D11_DEFAULT{} };
		//Description.DepthEnable = true;
		//RenderCommand::DX11GetDevice()->CreateDepthStencilState(&Description, &BasicDepthStencil_);
		//
		//D3D11_DEPTH_STENCIL_DESC Description2 = CD3D11_DEPTH_STENCIL_DESC{ CD3D11_DEFAULT{} };
		//Description2.DepthEnable = false;
		//Description2.DepthFunc = D3D11_COMPARISON_ALWAYS;
		//RenderCommand::DX11GetDevice()->CreateDepthStencilState(&Description2, &NoDepthStencil_);
	}
	void DX11RenderAPI::SetClearColour(float R, float G, float B, float A)
	{
		BackgroundColour[0] = R;
		BackgroundColour[1] = G;
		BackgroundColour[2] = B;
		BackgroundColour[3] = A;
	}

	void DX11RenderAPI::Clear()
	{
		Context->ClearRenderTargetView(RenderTarget.Get(), BackgroundColour);
		Context->ClearDepthStencilView(DepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0u);
	}

	void DX11RenderAPI::BeginFrame()
	{
		Clear();

		//Init Camera Projection	
		CameraComponent* MainCamera = Application::GetWorld()->GetLocalController()->GetCamera();
		MainCamera->SetProjection(DirectX::XMMatrixPerspectiveFovLH(MainCamera->GetFOV(), (float)ViewportSize_.X / (float)ViewportSize_.Y, NearPlane_, FarPlane_));
	}

	void DX11RenderAPI::EndFrame()
	{
		HRESULT ResultHandle;
		CATCH_ERROR_DX(SwapChain->Present(1, 0));

		Context->OMSetRenderTargets(1u, RenderTarget.GetAddressOf(), DepthStencilView.Get());
	}

	void DX11RenderAPI::DrawIndexed(int Count)
	{
		Context->DrawIndexed(Count, 0, 0);
	}

	void DX11RenderAPI::Resize(int Width, int Height)
	{
		HRESULT ResultHandle;

		ViewportSize_ = IntVector2D(Width, Height);

		Context->OMSetRenderTargets(0, 0, 0);
		RenderTarget->Release();

		CATCH_ERROR_DX(SwapChain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0));

		// Create render target view and populate backbuffer
		Microsoft::WRL::ComPtr<ID3D11Resource> BackBuffer = nullptr;
		CATCH_ERROR_DX(SwapChain->GetBuffer(0, __uuidof(ID3D11Resource), &BackBuffer));
		CATCH_ERROR_DX(Device->CreateRenderTargetView(BackBuffer.Get(), nullptr, RenderTarget.GetAddressOf()));

		DepthStencilView->Release();

		Microsoft::WRL::ComPtr<ID3D11Texture2D> DepthStencil = nullptr;
		D3D11_TEXTURE2D_DESC DepthDescription = {};
		DepthDescription.Width = Width;
		DepthDescription.Height = Height;
		DepthDescription.MipLevels = 1u;
		DepthDescription.ArraySize = 1u;
		DepthDescription.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		DepthDescription.SampleDesc.Count = 1u;
		DepthDescription.SampleDesc.Quality = 0u;
		DepthDescription.Usage = D3D11_USAGE_DEFAULT;
		DepthDescription.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		CATCH_ERROR_DX(Device->CreateTexture2D(&DepthDescription, nullptr, &DepthStencil));

		// Create the depth stencil view
		D3D11_DEPTH_STENCIL_VIEW_DESC DepthStencilViewDescription = {};
		DepthStencilViewDescription.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		DepthStencilViewDescription.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		DepthStencilViewDescription.Texture2D.MipSlice = 0u;
		CATCH_ERROR_DX(Device->CreateDepthStencilView(DepthStencil.Get(), &DepthStencilViewDescription, DepthStencilView.GetAddressOf()));

		Context->OMSetRenderTargets(1u, RenderTarget.GetAddressOf(), DepthStencilView.Get());

		//Setup Viewport
		D3D11_VIEWPORT Viewport;
		Viewport.Width = (FLOAT)Width;
		Viewport.Height = (FLOAT)Height;
		Viewport.MinDepth = 0.0f;
		Viewport.MaxDepth = 1.0f; //Disable depth for testing
		Viewport.TopLeftX = 0.0f;
		Viewport.TopLeftY = 0.0f;

		Context->RSSetViewports(1u, &Viewport);

		CameraComponent* MainCamera = Application::GetWorld()->GetLocalController()->GetCamera();
		MainCamera->SetProjection(DirectX::XMMatrixPerspectiveFovLH(MainCamera->GetFOV(), (float)ViewportSize_.X / (float)ViewportSize_.Y, NearPlane_, FarPlane_));
	}

	void DX11RenderAPI::EnableDepth()
	{
		RenderCommand::DX11GetContext()->OMSetDepthStencilState(BasicDepthStencil_.Get(), 0xFF);
	}

	void DX11RenderAPI::DisableDepth()
	{
		//RenderCommand::DX11GetContext()->OMSetDepthStencilState(NoDepthStencil_.Get(), 0xFF);
		RenderCommand::DX11GetContext()->OMSetDepthStencilState(NoDepthStencil_.Get(), 1);
	}

	Vector DX11RenderAPI::GetScreenToWorldDirection(int X, int Y)
	{
		CameraComponent* MainCamera = Application::GetWorld()->GetLocalController()->GetCamera();

		IntVector2D WinSize = WinWindow::GetAdjustedWindowSize();
		float Width = WinSize.X;
		float Height = WinSize.Y;

		//Normalise into +1 to -1
		float MouseX = ((2 * X) / Width) - 1;
		float MouseY = -(((2 * Y) / Height) - 1);

		//FLOW_ENGINE_ERROR("X: {0}, Y: {1}", MouseX, MouseY);

		//Adjust for the projection matrix
		DirectX::XMFLOAT4X4 Projection;
		DirectX::XMStoreFloat4x4(&Projection, MainCamera->GetProjectionMatrix());
		MouseX = MouseX / Projection._11;
		MouseY = MouseY / Projection._22;		

		// Get the inverse of the view matrix.
		DirectX::XMMATRIX InverseMatrix = DirectX::XMMatrixInverse(nullptr, RenderCommand::GetCamera().GetViewMatrix());
		DirectX::XMFLOAT4X4 Inverse;
		DirectX::XMStoreFloat4x4(&Inverse, InverseMatrix);

		// Calculate the direction of the picking ray in view space.
		Vector Direction;
		Direction.X = (MouseX * Inverse._11) + (MouseY * Inverse._21) + Inverse._31;
		Direction.Y = (MouseX * Inverse._12) + (MouseY * Inverse._22) + Inverse._32;
		Direction.Z = (MouseX * Inverse._13) + (MouseY * Inverse._23) + Inverse._33;

		//TODO: Normalise direction
		//FLOW_ENGINE_LOG("Direction: {0}", Direction);

		return Direction;
	}

	ID3D11Device* DX11RenderAPI::GetDevice()
	{
		return Device.Get();
	}

	ID3D11DeviceContext* DX11RenderAPI::GetContext()
	{
		return Context.Get();
	}

	DirectX::XMMATRIX DX11RenderAPI::GetOrthographicMatrix()
	{
		return DirectX::XMMatrixOrthographicLH((float)ViewportSize_.X, (float)ViewportSize_.Y, NearPlane_, FarPlane_);
	}
}