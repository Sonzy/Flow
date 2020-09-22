#include "Flowpch.h"
#include "DX11RenderAPI.h"

#include "Flow\Rendering\RenderCommand.h"
#include "Flow\Application.h"
#include "Flow\Window\WinWindow.h"

#include "Flow/Rendering/Core/Camera/Camera.h"
#include "Flow/Rendering/Other/FrameBuffer.h"
#include "Flow/Rendering/Other/DepthBuffer.h"

#if WITH_EDITOR
#include "Flow/Editor/EditorLayer.h"
#endif


#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")
#pragma comment(lib, "dxguid.lib")

DX11RenderAPI::~DX11RenderAPI()
{
}

void DX11RenderAPI::InitialiseDX11API(HWND WindowHandle, int ViewportWidth, int ViewportHeight)
{
	_NearPlane = 0.5f;
	_FarPlane = 2000.0f;
	_ViewportSize = IntVector2(ViewportWidth, ViewportHeight);

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
			_SwapChain.GetAddressOf(),
			_Device.GetAddressOf(),
			nullptr,
			_Context.GetAddressOf())
	);

	//CATCH_ERROR_DX(_Device->QueryInterface(__uuidof(ID3D11Debug), (void**)&_DeviceDebug));

	// Create render target view and populate backbuffer
	Microsoft::WRL::ComPtr<ID3D11Resource> BackBuffer = nullptr;
	CATCH_ERROR_DX(_SwapChain->GetBuffer(0, __uuidof(ID3D11Resource), &BackBuffer));
	CATCH_ERROR_DX(_Device->CreateRenderTargetView(BackBuffer.Get(), nullptr, &_RenderTarget));

	ResizeDepthBuffer(ViewportWidth, ViewportHeight);

	// Bind the Depth Stencil to the Output Merger
	_Context->OMSetRenderTargets(1u, _RenderTarget.GetAddressOf(), _DepthTextureView.Get());

	//Setup Viewport
	D3D11_VIEWPORT Viewport;
	Viewport.Width = (FLOAT)ViewportWidth;
	Viewport.Height = (FLOAT)ViewportHeight;
	Viewport.MinDepth = 0.0f;
	Viewport.MaxDepth = 1.0f;
	Viewport.TopLeftX = 0.0f;
	Viewport.TopLeftY = 0.0f;

	_Context->RSSetViewports(1u, &Viewport);

#if WITH_EDITOR
	_EditorBuffer = new FrameBuffer(ViewportWidth, ViewportHeight, true);
#endif

	CurrentBuffer = nullptr;
}
void DX11RenderAPI::SetClearColour(float R, float G, float B, float A)
{
	_BackgroundColour[0] = R;
	_BackgroundColour[1] = G;
	_BackgroundColour[2] = B;
	_BackgroundColour[3] = A;
}

void DX11RenderAPI::Clear()
{
	_Context->ClearRenderTargetView(_RenderTarget.Get(), _BackgroundColour);
	_Context->ClearDepthStencilView(_DepthTextureView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0u);
}

void DX11RenderAPI::BeginFrame()
{
	Clear();

	_MainCamera->SetProjectionMatrix(DirectX::XMMatrixPerspectiveFovLH(Maths::DegreesToRadians(_MainCamera->GetFOV()), (float)_ViewportSize.x / (float)_ViewportSize.y, _NearPlane, _FarPlane));
}

void DX11RenderAPI::EndFrame()
{
	HRESULT ResultHandle;
	CATCH_ERROR_DX(_SwapChain->Present(0, 0));

	_Context->OMSetRenderTargets(1u, _RenderTarget.GetAddressOf(), _DepthTextureView.Get());
}

void DX11RenderAPI::DrawIndexed(int Count)
{
	//TODO: Allow for unsigned int type instead, cba right now
	_Context->DrawIndexed(Count, 0, 0);
}

void DX11RenderAPI::Draw(unsigned int Count)
{
	_Context->Draw(Count, 0);
}

void DX11RenderAPI::Resize(int Width, int Height)
{
	//TODO: Shouldnt have to do anything different for editor since the windows messages should be processed befoer the scene is rendered
	HRESULT ResultHandle;

	_ViewportSize = IntVector2(Width, Height);

	_Context->OMSetRenderTargets(0, 0, 0);
	_RenderTarget->Release();
	//_DeviceDebug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);

	CATCH_ERROR_DX(_SwapChain->ResizeBuffers(0, Width, Height, DXGI_FORMAT_UNKNOWN, 0));

	//// Create render target view and populate backbuffer
	Microsoft::WRL::ComPtr<ID3D11Resource> BackBuffer = nullptr;
	CATCH_ERROR_DX(_SwapChain->GetBuffer(0, __uuidof(ID3D11Resource), &BackBuffer));
	CATCH_ERROR_DX(_Device->CreateRenderTargetView(BackBuffer.Get(), nullptr, &_RenderTarget));

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
	CATCH_ERROR_DX(_Device->CreateTexture2D(&DepthDescription, nullptr, &_DepthTexture));

	// Create the depth stencil view
	D3D11_DEPTH_STENCIL_VIEW_DESC DepthStencilViewDescription = {};
	DepthStencilViewDescription.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	DepthStencilViewDescription.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	DepthStencilViewDescription.Texture2D.MipSlice = 0u;
	CATCH_ERROR_DX(_Device->CreateDepthStencilView(_DepthTexture.Get(), &DepthStencilViewDescription, &_DepthTextureView));

	_Context->OMSetRenderTargets(1u, _RenderTarget.GetAddressOf(), _DepthTextureView.Get());

	//Setup Viewport
	D3D11_VIEWPORT Viewport;
	Viewport.Width = (FLOAT)Width;
	Viewport.Height = (FLOAT)Height;
	Viewport.MinDepth = 0.0f;
	Viewport.MaxDepth = 1.0f; //Disable depth for testing
	Viewport.TopLeftX = 0.0f;
	Viewport.TopLeftY = 0.0f;

	_Context->RSSetViewports(1u, &Viewport);

	_MainCamera->SetProjectionMatrix(DirectX::XMMatrixPerspectiveFovLH(Maths::DegreesToRadians(_MainCamera->GetFOV()), (float)_ViewportSize.x / (float)_ViewportSize.y, _NearPlane, _FarPlane));

	//Note: See ScreenToWorldVec, will fix properly another time
#if WITH_EDITOR
	if (CurrentBuffer == _EditorBuffer)
		_MainCamera->SetSceneProjection(DirectX::XMMatrixPerspectiveFovLH(Maths::DegreesToRadians(_MainCamera->GetFOV()), (float)_ViewportSize.x / (float)_ViewportSize.y, _NearPlane, _FarPlane));
#endif
}

void DX11RenderAPI::ResizeDepthBuffer(int Width, int Height)
{
	HRESULT ResultHandle;

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
	CATCH_ERROR_DX(_Device->CreateTexture2D(&DepthDescription, nullptr, &_DepthTexture));

	// Create the depth stencil view
	D3D11_DEPTH_STENCIL_VIEW_DESC DepthStencilViewDescription = {};
	DepthStencilViewDescription.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	DepthStencilViewDescription.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	DepthStencilViewDescription.Texture2D.MipSlice = 0u;
	CATCH_ERROR_DX(_Device->CreateDepthStencilView(_DepthTexture.Get(), &DepthStencilViewDescription, &_DepthTextureView));
}

Vector3 DX11RenderAPI::GetScreenToWorldDirection(int X, int Y, IntVector2 WindowSize, IntVector2 Origin)
{
	//TODO: need to pass in the window size instead incase im checking from any other window
#if WITH_EDITOR
	IntVector2 WinSize = EditorLayer::GetEditor()->GetSceneWindowSize();
#else
	IntVector2 WinSize = WinWindow::GetAdjustedWindowSize();
#endif

	float Width = (float)WindowSize.x;
	float Height = (float)WindowSize.y;

	//Normalise into +1 to -1
	float MouseX = ((2 * (X - Origin.x)) / Width) - 1;
	float MouseY = -(((2 * (Y - Origin.y)) / Height) - 1);

	//FLOW_ENGINE_ERROR("X: {0}, Y: {1}", MouseX, MouseY);

	//Adjust for the projection matrix
	DirectX::XMFLOAT4X4 Projection;

#if WITH_EDITOR
	//Note: Patch fix, since input events are fired before the scene begins, the projection matrix is incorrect in the editor
	//		since the current projection is fullscreen. So were lazy and grab a cached version from the previous frame
	DirectX::XMStoreFloat4x4(&Projection, _MainCamera->GetSceneProjectionMatrix());	
#else
	DirectX::XMStoreFloat4x4(&Projection, _MainCamera->GetProjectionMatrix());
#endif

	MouseX = MouseX / Projection._11;
	MouseY = MouseY / Projection._22;

	// Get the inverse of the view matrix.
	DirectX::XMMATRIX InverseMatrix = DirectX::XMMatrixInverse(nullptr, RenderCommand::GetMainCamera()->GetViewMatrix());
	DirectX::XMFLOAT4X4 Inverse;
	DirectX::XMStoreFloat4x4(&Inverse, InverseMatrix);

	// Calculate the direction of the picking ray in view space.
	Vector3 Direction;
	Direction.x = (MouseX * Inverse._11) + (MouseY * Inverse._21) + Inverse._31;
	Direction.y = (MouseX * Inverse._12) + (MouseY * Inverse._22) + Inverse._32;
	Direction.z = (MouseX * Inverse._13) + (MouseY * Inverse._23) + Inverse._33;

	//TODO: Normalise direction
	//FLOW_ENGINE_LOG("Direction: {0}", Direction);

	return Direction;
}

ID3D11Device* DX11RenderAPI::GetDevice()
{
	return _Device.Get();
}

ID3D11DeviceContext* DX11RenderAPI::GetContext()
{
	return _Context.Get();
}

void DX11RenderAPI::BindBackBuffer()
{
	CREATE_RESULT_HANDLE();

	// Create render target view and populate backbuffer
	Microsoft::WRL::ComPtr<ID3D11Resource> BackBuffer = nullptr;
	CATCH_ERROR_DX(_SwapChain->GetBuffer(0, __uuidof(ID3D11Resource), &BackBuffer));
	CATCH_ERROR_DX(_Device->CreateRenderTargetView(BackBuffer.Get(), nullptr, &_RenderTarget));

	Window& Win = Application::Get().GetWindow();
	IntVector2 AdjWindowSize = dynamic_cast<WinWindow&>(Win).GetAdjustedWindowSize();
	_ViewportSize = { AdjWindowSize.x, AdjWindowSize.y };

	//ResizeDepthBuffer(_ViewportSize.X, _ViewportSize.Y);

	_Context->OMSetRenderTargets(1u, _RenderTarget.GetAddressOf(), _DepthTextureView.Get());

	D3D11_VIEWPORT Viewport;
	Viewport.Width = (FLOAT)_ViewportSize.x;
	Viewport.Height = (FLOAT)_ViewportSize.y;
	Viewport.MinDepth = 0.0f;
	Viewport.MaxDepth = 1.0f;
	Viewport.TopLeftX = 0.0f;
	Viewport.TopLeftY = 0.0f;

	_Context->RSSetViewports(1u, &Viewport);
 
#if WITH_EDITOR
	_EditorBufferBound = false;
#endif
}

void DX11RenderAPI::BindFrameBuffer(FrameBuffer* Buffer)
{
	CREATE_RESULT_HANDLE();

	//Create Render Target view and bind framebuffer
	CATCH_ERROR_DX(_Device->CreateRenderTargetView(Buffer->GetTexture(), nullptr, &_RenderTarget));

	_ViewportSize = { (int)Buffer->GetWidth(), (int)Buffer->GetHeight()};
	if (!Buffer->HasDepthBuffer())
	{
		ResizeDepthBuffer(_ViewportSize.x, _ViewportSize.y);
		_Context->OMSetRenderTargets(1u, _RenderTarget.GetAddressOf(), _DepthTextureView.Get());
	}
	else
	{
		_Context->OMSetRenderTargets(1u, _RenderTarget.GetAddressOf(), Buffer->GetDepthBuffer()->GetTextureView());

		_Context->ClearRenderTargetView(_RenderTarget.Get(), _BackgroundColour);
		_Context->ClearDepthStencilView(Buffer->GetDepthBuffer()->GetTextureView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0u);
	}

	D3D11_VIEWPORT Viewport;
	Viewport.Width = (FLOAT)Buffer->GetWidth();
	Viewport.Height = (FLOAT)Buffer->GetHeight();
	Viewport.MinDepth = 0.0f;
	Viewport.MaxDepth = 1.0f;
	Viewport.TopLeftX = 0.0f;
	Viewport.TopLeftY = 0.0f;

	_Context->RSSetViewports(1u, &Viewport);

	CurrentBuffer = Buffer;
#if WITH_EDITOR
	_EditorBufferBound = false;
#endif
}

void DX11RenderAPI::BindEditorFrameBuffer()
{
	//Bind the frame buffer
	BindFrameBuffer(_EditorBuffer);
	_EditorBufferBound = true;

	//Update the window rendering properties
	_MainCamera->SetProjectionMatrix(DirectX::XMMatrixPerspectiveFovLH(Maths::DegreesToRadians(_MainCamera->GetFOV()), (float)_ViewportSize.x / (float)_ViewportSize.y, _NearPlane, _FarPlane));
	_MainCamera->SetSceneProjection(DirectX::XMMatrixPerspectiveFovLH(Maths::DegreesToRadians(_MainCamera->GetFOV()), (float)_ViewportSize.x / (float)_ViewportSize.y, _NearPlane, _FarPlane));
}

FrameBuffer* DX11RenderAPI::GetEditorBuffer() const
{
	return _EditorBuffer;
}
