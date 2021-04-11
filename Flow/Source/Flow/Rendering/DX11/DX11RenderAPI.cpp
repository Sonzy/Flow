// Pch ///////////////////////////////////////////////////////////////

#include "pch.h"

// Includes //////////////////////////////////////////////////////////
#include "DX11RenderAPI.h"

#include "Application.h"
#include "Framework/Utils/DirectX11/DirectX11Utils.h"
#include "Rendering/Core/Camera/Camera.h"
#include "Rendering/Other/FrameBuffer.h"
#include "Rendering/Other/DepthBuffer.h"
#include "Window/Window_Win32.h"

#if WITH_EDITOR
	#include "Editor/Editor.h"
#endif


// Libraries /////////////////////////////////////////////////////////

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")
#pragma comment(lib, "dxguid.lib")

// Function Definitions //////////////////////////////////////////////

DX11RenderAPI::~DX11RenderAPI()
{
	m_swapChain.Reset();
	m_context.Reset();
	m_renderTarget.Reset();
	m_device.Reset();
	m_depthTexture.Reset();
	m_depthTextureView.Reset();
	m_deviceDebug.Reset();


#if WITH_EDITOR
	delete m_editorBuffer;
#endif
	delete m_currentBuffer;
}

void DX11RenderAPI::Initialise(HWND WindowHandle, int ViewportWidth, int ViewportHeight)
{
	m_nearPlane = 0.05f;
	m_farPlane = 2000.0f;
	m_viewportSize = IntVector2(ViewportWidth, ViewportHeight);
	
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
	SwapchainDesc.BufferUsage = D3D11_BIND_RENDER_TARGET;
	SwapchainDesc.BufferCount = 2; //flip discard
	SwapchainDesc.OutputWindow = WindowHandle;
	SwapchainDesc.Windowed = TRUE;
	SwapchainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD; //flip discard
	SwapchainDesc.Flags = 0;

	UINT SwapCreateFlags = 0;
	SwapCreateFlags |= D3D11_CREATE_DEVICE_DEBUG; //TODO: Disable in non debug mode

	CaptureDXError(
		D3D11CreateDeviceAndSwapChain(
			nullptr,
			D3D_DRIVER_TYPE_HARDWARE,
			nullptr,
			SwapCreateFlags,
			nullptr,
			0,
			D3D11_SDK_VERSION,
			&SwapchainDesc,
			&m_swapChain,
			&m_device,
			nullptr,
			&m_context)
	);

	//CaptureDXError(_Device->QueryInterface(__uuidof(ID3D11Debug), (void**)&_DeviceDebug));

	// Create render target view and populate backbuffer
	ComPtr<ID3D11Resource> BackBuffer = nullptr;
	CaptureDXError(m_swapChain->GetBuffer(0, __uuidof(ID3D11Resource), &BackBuffer));
	CaptureDXError(m_device->CreateRenderTargetView(BackBuffer.Get(), nullptr, &m_renderTarget));

	ResizeDepthBuffer(ViewportWidth, ViewportHeight);

	// Bind the Depth Stencil to the Output Merger
	m_context->OMSetRenderTargets(1u, m_renderTarget.GetAddressOf(), m_depthTextureView.Get());

	//Setup Viewport
	D3D11_VIEWPORT Viewport;
	Viewport.Width = (FLOAT)ViewportWidth;
	Viewport.Height = (FLOAT)ViewportHeight;
	Viewport.MinDepth = 0.0f;
	Viewport.MaxDepth = 1.0f;
	Viewport.TopLeftX = 0.0f;
	Viewport.TopLeftY = 0.0f;

	m_context->RSSetViewports(1u, &Viewport);

#if WITH_EDITOR
	m_editorBuffer = new FrameBuffer("Editor Buffer", ViewportWidth, ViewportHeight, true);
#endif
	m_gameBuffer = new FrameBuffer("Game Buffer", ViewportWidth, ViewportHeight, true); //Keep this the correct size

	m_currentBuffer = nullptr;
}

void DX11RenderAPI::SetClearColour(float R, float G, float B, float A)
{
	m_backgroundColour[0] = R;
	m_backgroundColour[1] = G;
	m_backgroundColour[2] = B;
	m_backgroundColour[3] = A;
}

void DX11RenderAPI::Clear()
{
	m_context->ClearRenderTargetView(m_renderTarget.Get(), m_backgroundColour);
	m_context->ClearDepthStencilView(m_depthTextureView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0u);
}

void DX11RenderAPI::BeginFrame()
{
	if (m_windowMinimised == true)
	{
		return;
	}

	Clear();

	CameraBase* mainCamera = Renderer::GetMainCamera();
	mainCamera->SetProjectionMatrix(DirectX::XMMatrixPerspectiveFovLH(Maths::DegreesToRadians(mainCamera->GetFOV()), (float)m_viewportSize.x / (float)m_viewportSize.y, m_nearPlane, m_farPlane));
}

void DX11RenderAPI::EndFrame()
{
	if (m_windowMinimised == true)
	{
		return;
	}

	HRESULT ResultHandle;
	CaptureDXError(m_swapChain->Present(0, 0));

	m_context->OMSetRenderTargets(1u, m_renderTarget.GetAddressOf(), m_depthTextureView.Get());
}

void DX11RenderAPI::DrawIndexed(int Count)
{
	//TODO: Allow for unsigned int type instead, cba right now
	m_context->DrawIndexed(Count, 0, 0);
}

void DX11RenderAPI::Draw(unsigned int Count)
{
	m_context->Draw(Count, 0);
}

void DX11RenderAPI::Resize(int Width, int Height)
{
	assert((Width != 0 && Height != 0) && "Width or Height was invalid");

	//TODO: Shouldnt have to do anything different for editor since the windows messages should be processed befoer the scene is rendered
	HRESULT ResultHandle;

	m_viewportSize = IntVector2(Width, Height);

	m_context->OMSetRenderTargets(0, 0, 0);
	m_renderTarget = nullptr;

	//_DeviceDebug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);

	CaptureDXError(m_swapChain->ResizeBuffers(0, Width, Height, DXGI_FORMAT_UNKNOWN, 0));

	//// Create render target view and populate backbuffer
	Microsoft::WRL::ComPtr<ID3D11Resource> BackBuffer = nullptr;
	CaptureDXError(m_swapChain->GetBuffer(0, __uuidof(ID3D11Resource), &BackBuffer));
	CaptureDXError(m_device->CreateRenderTargetView(BackBuffer.Get(), nullptr, &m_renderTarget));
	BackBuffer.Reset();

	ResizeDepthBuffer(Width, Height);

	m_context->OMSetRenderTargets(1u, m_renderTarget.GetAddressOf(), m_depthTextureView.Get());

	//Setup Viewport
	D3D11_VIEWPORT Viewport;
	Viewport.Width = (FLOAT)Width;
	Viewport.Height = (FLOAT)Height;
	Viewport.MinDepth = 0.0f;
	Viewport.MaxDepth = 1.0f; //Disable depth for testing
	Viewport.TopLeftX = 0.0f;
	Viewport.TopLeftY = 0.0f;

	m_context->RSSetViewports(1u, &Viewport);

	//VerticalFOV = HorizontalFOV / AspectRatio.
	Renderer::GetMainCamera()->SetProjectionMatrix(DirectX::XMMatrixPerspectiveFovLH(Maths::DegreesToRadians(Renderer::GetMainCamera()->GetFOV()), (float)m_viewportSize.x / (float)m_viewportSize.y, m_nearPlane, m_farPlane));
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
	CaptureDXError(m_device->CreateTexture2D(&DepthDescription, nullptr, &m_depthTexture));

	// Create the depth stencil view
	D3D11_DEPTH_STENCIL_VIEW_DESC DepthStencilViewDescription = {};
	DepthStencilViewDescription.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	DepthStencilViewDescription.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	DepthStencilViewDescription.Texture2D.MipSlice = 0u;
	CaptureDXError(m_device->CreateDepthStencilView(m_depthTexture.Get(), &DepthStencilViewDescription, &m_depthTextureView));
}

void DX11RenderAPI::SetProjectionPerspectiveMatrixDefault()
{
	Renderer::GetMainCamera()->SetProjectionMatrix(DirectX::XMMatrixPerspectiveFovLH(Maths::DegreesToRadians(Renderer::GetMainCamera()->GetFOV()), (float)m_viewportSize.x / (float)m_viewportSize.y, m_nearPlane, m_farPlane));
}

void DX11RenderAPI::SetProjectionOrthographicMatrixDefault()
{
	Renderer::GetMainCamera()->SetProjectionMatrix(DirectX::XMMatrixOrthographicLH((float)m_viewportSize.x, (float)m_viewportSize.y, m_nearPlane, m_farPlane));
}

Vector3 DX11RenderAPI::GetScreenToWorldDirection(int X, int Y, IntVector2 WindowSize, IntVector2 Origin)
{
	//TODO: need to pass in the window size instead incase im checking from any other window
#if WITH_EDITOR
	IntVector2 WinSize = Editor::Get().GetSceneWindowSize();
#else
	IntVector2 WinSize = Window_Win32::GetAdjustedWindowSize();
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
	DirectX::XMStoreFloat4x4(&Projection, Renderer::GetMainCamera()->GetSceneProjectionMatrix());	
#else
	DirectX::XMStoreFloat4x4(&Projection, Renderer::GetMainCamera()->GetProjectionMatrix());
#endif

	MouseX = MouseX / Projection._11;
	MouseY = MouseY / Projection._22;

	// Get the inverse of the view matrix.
	DirectX::XMMATRIX InverseMatrix = DirectX::XMMatrixInverse(nullptr, Renderer::GetMainCamera()->GetViewMatrix());
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

IntVector2 DX11RenderAPI::WorldToScreen(Vector3 position)
{
	CameraBase* cam = Renderer::GetMainCamera();
	Vector3 camPosition = cam->GetCameraPosition();

	DirectX::XMVECTOR vecPosition = DirectX::XMVectorSet(position.x, position.y, position.z, 1.0f);
	DirectX::XMVECTOR outProjection = DirectX::XMVector3Project(
		vecPosition,
		0.0f, 0.0f,
		(float)m_viewportSize.x, (float)m_viewportSize.y,
		0.0f, 1.0f,
		cam->GetProjectionMatrix(),
		cam->GetViewMatrix(),
		DirectX::XMMatrixIdentity()
	);

	DirectX::XMFLOAT4 outVec;
	DirectX::XMStoreFloat4(&outVec, outProjection);

	//TODO: Remove in the future
	if (outVec.z > 1.0f)
	{
		outVec.x = -999.9f;
		outVec.y = -999.9f;
	}

	return IntVector2(outVec.x, outVec.y);
}

ID3D11Device* DX11RenderAPI::GetDevice()
{
	return m_device.Get();
}

ID3D11DeviceContext* DX11RenderAPI::GetContext()
{
	return m_context.Get();
}

void DX11RenderAPI::BindBackBuffer(bool clear)
{
	CreateResultHandle();

	// Create render target view and populate backbuffer
	Microsoft::WRL::ComPtr<ID3D11Resource> BackBuffer = nullptr;
	CaptureDXError(m_swapChain->GetBuffer(0, __uuidof(ID3D11Resource), &BackBuffer));
	CaptureDXError(m_device->CreateRenderTargetView(BackBuffer.Get(), nullptr, &m_renderTarget));

	Window& Win = Application::Get().GetWindow();
	IntVector2 AdjWindowSize = dynamic_cast<Window_Win32&>(Win).GetAdjustedWindowSize();
	m_viewportSize = { AdjWindowSize.x, AdjWindowSize.y };

	//ResizeDepthBuffer(_ViewportSize.X, _ViewportSize.Y);

	m_context->OMSetRenderTargets(1u, m_renderTarget.GetAddressOf(), m_depthTextureView.Get());

	D3D11_VIEWPORT Viewport;
	Viewport.Width = (FLOAT)m_viewportSize.x;
	Viewport.Height = (FLOAT)m_viewportSize.y;
	Viewport.MinDepth = 0.0f;
	Viewport.MaxDepth = 1.0f;
	Viewport.TopLeftX = 0.0f;
	Viewport.TopLeftY = 0.0f;

	m_context->RSSetViewports(1u, &Viewport);

#if WITH_EDITOR
	m_editorBufferBound = false;
#endif
}

void DX11RenderAPI::BindFrameBuffer(FrameBuffer* Buffer, bool clear)
{
	CreateResultHandle();

	//Create Render Target view and bind framebuffer
	CaptureDXError(m_device->CreateRenderTargetView(Buffer->GetTexture(), nullptr, &m_renderTarget));

	m_viewportSize = { (int)Buffer->GetWidth(), (int)Buffer->GetHeight() };

	if (clear == true)
	{
		if (!Buffer->HasDepthBuffer())
		{
			ResizeDepthBuffer(m_viewportSize.x, m_viewportSize.y);
			m_context->OMSetRenderTargets(1u, m_renderTarget.GetAddressOf(), m_depthTextureView.Get());
		}
		else
		{
			m_context->OMSetRenderTargets(1u, m_renderTarget.GetAddressOf(), Buffer->GetDepthBuffer()->GetTextureView());

			m_context->ClearRenderTargetView(m_renderTarget.Get(), m_backgroundColour);
			m_context->ClearDepthStencilView(Buffer->GetDepthBuffer()->GetTextureView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0u);
		}
	}

	D3D11_VIEWPORT Viewport;
	Viewport.Width = (FLOAT)Buffer->GetWidth();
	Viewport.Height = (FLOAT)Buffer->GetHeight();
	Viewport.MinDepth = 0.0f;
	Viewport.MaxDepth = 1.0f;
	Viewport.TopLeftX = 0.0f;
	Viewport.TopLeftY = 0.0f;

	m_context->RSSetViewports(1u, &Viewport);

	m_currentBuffer = Buffer;
#if WITH_EDITOR
	m_editorBufferBound = false;
#endif
}

#if WITH_EDITOR
void DX11RenderAPI::BindEditorFrameBuffer(bool clear)
{
	//Bind the frame buffer
	BindFrameBuffer(m_editorBuffer, clear);
	m_editorBufferBound = true;

	//Update the window rendering properties
	CameraBase* mainCamera = Renderer::GetMainCamera();
	mainCamera->SetProjectionMatrix(DirectX::XMMatrixPerspectiveFovLH(Maths::DegreesToRadians(mainCamera->GetFOV()), (float)m_viewportSize.x / (float)m_viewportSize.y, m_nearPlane, m_farPlane));
}

FrameBuffer* DX11RenderAPI::GetEditorBuffer() const
{
	return m_editorBuffer;
}

#endif // WITH_EDITOR

void DX11RenderAPI::BindGameFrameBuffer(bool clear)
{
	//Bind the frame buffer
	BindFrameBuffer(m_gameBuffer, clear);

#if WITH_EDITOR
	m_editorBufferBound = false;
#endif // WITH_EDITOR

	//Update the window rendering properties
	CameraBase* mainCamera = Renderer::GetMainCamera();
	mainCamera->SetProjectionMatrix(DirectX::XMMatrixPerspectiveFovLH(Maths::DegreesToRadians(mainCamera->GetFOV()), (float)m_viewportSize.x / (float)m_viewportSize.y, m_nearPlane, m_farPlane));
}

FrameBuffer* DX11RenderAPI::GetGameBuffer() const
{
	return m_gameBuffer;
}