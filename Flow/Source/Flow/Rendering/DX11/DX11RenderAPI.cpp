#include "Flowpch.h"
#include "DX11RenderAPI.h"

#include "Flow\Rendering\RenderCommand.h"
#include "Flow\Application.h"
#include "Flow\Window\Window_Win32.h"

#include "Flow/Rendering/Core/Camera/Camera.h"
#include "Flow/Rendering/Other/FrameBuffer.h"
#include "Flow/Rendering/Other/DepthBuffer.h"

#if WITH_EDITOR
#include "Flow/Editor/Editor.h"
#endif


#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")
#pragma comment(lib, "dxguid.lib")

DX11RenderAPI::~DX11RenderAPI()
{
	m_SwapChain.Reset();
	m_Context.Reset();
	m_RenderTarget.Reset();
	m_Device.Reset();
	m_DepthTexture.Reset();
	m_DepthTextureView.Reset();
	m_DeviceDebug.Reset();



#if WITH_EDITOR
	if (m_EditorBuffer != nullptr)
	{
		delete m_EditorBuffer;
	}
#endif

	if (m_CurrentBuffer != m_EditorBuffer && m_CurrentBuffer != nullptr)
	{
		delete m_CurrentBuffer;
	}
}

void DX11RenderAPI::InitialiseDX11API(HWND WindowHandle, int ViewportWidth, int ViewportHeight)
{
	m_NearPlane = 0.05f;
	m_FarPlane = 2000.0f;
	m_ViewportSize = IntVector2(ViewportWidth, ViewportHeight);
	
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
			m_SwapChain.GetAddressOf(),
			m_Device.GetAddressOf(),
			nullptr,
			m_Context.GetAddressOf())
	);

	//CATCH_ERROR_DX(_Device->QueryInterface(__uuidof(ID3D11Debug), (void**)&_DeviceDebug));

	// Create render target view and populate backbuffer
	Microsoft::WRL::ComPtr<ID3D11Resource> BackBuffer = nullptr;
	CATCH_ERROR_DX(m_SwapChain->GetBuffer(0, __uuidof(ID3D11Resource), &BackBuffer));
	CATCH_ERROR_DX(m_Device->CreateRenderTargetView(BackBuffer.Get(), nullptr, &m_RenderTarget));

	ResizeDepthBuffer(ViewportWidth, ViewportHeight);

	// Bind the Depth Stencil to the Output Merger
	m_Context->OMSetRenderTargets(1u, m_RenderTarget.GetAddressOf(), m_DepthTextureView.Get());

	//Setup Viewport
	D3D11_VIEWPORT Viewport;
	Viewport.Width = (FLOAT)ViewportWidth;
	Viewport.Height = (FLOAT)ViewportHeight;
	Viewport.MinDepth = 0.0f;
	Viewport.MaxDepth = 1.0f;
	Viewport.TopLeftX = 0.0f;
	Viewport.TopLeftY = 0.0f;

	m_Context->RSSetViewports(1u, &Viewport);

#if WITH_EDITOR
	m_EditorBuffer = new FrameBuffer(ViewportWidth, ViewportHeight, true);
#endif

	m_CurrentBuffer = nullptr;
}

void DX11RenderAPI::SetClearColour(float R, float G, float B, float A)
{
	m_BackgroundColour[0] = R;
	m_BackgroundColour[1] = G;
	m_BackgroundColour[2] = B;
	m_BackgroundColour[3] = A;
}

void DX11RenderAPI::Clear()
{
	m_Context->ClearRenderTargetView(m_RenderTarget.Get(), m_BackgroundColour);
	m_Context->ClearDepthStencilView(m_DepthTextureView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0u);
}

void DX11RenderAPI::BeginFrame()
{
	if (m_WindowMinimised == true)
	{
		return;
	}

	Clear();

	m_MainCamera->SetProjectionMatrix(DirectX::XMMatrixPerspectiveFovLH(Maths::DegreesToRadians(m_MainCamera->GetFOV()), (float)m_ViewportSize.x / (float)m_ViewportSize.y, m_NearPlane, m_FarPlane));
}

void DX11RenderAPI::EndFrame()
{
	if (m_WindowMinimised == true)
	{
		return;
	}

	HRESULT ResultHandle;
	CATCH_ERROR_DX(m_SwapChain->Present(0, 0));

	m_Context->OMSetRenderTargets(1u, m_RenderTarget.GetAddressOf(), m_DepthTextureView.Get());
}

void DX11RenderAPI::DrawIndexed(int Count)
{
	//TODO: Allow for unsigned int type instead, cba right now
	m_Context->DrawIndexed(Count, 0, 0);
}

void DX11RenderAPI::Draw(unsigned int Count)
{
	m_Context->Draw(Count, 0);
}

void DX11RenderAPI::Resize(int Width, int Height)
{
	assert((Width != 0 && Height != 0) && "Width or Height was invalid");

	//TODO: Shouldnt have to do anything different for editor since the windows messages should be processed befoer the scene is rendered
	HRESULT ResultHandle;

	m_ViewportSize = IntVector2(Width, Height);

	m_Context->OMSetRenderTargets(0, 0, 0);
	m_RenderTarget.Reset();
	//_DeviceDebug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);

	CATCH_ERROR_DX(m_SwapChain->ResizeBuffers(0, Width, Height, DXGI_FORMAT_UNKNOWN, 0));

	//// Create render target view and populate backbuffer
	Microsoft::WRL::ComPtr<ID3D11Resource> BackBuffer = nullptr;
	CATCH_ERROR_DX(m_SwapChain->GetBuffer(0, __uuidof(ID3D11Resource), &BackBuffer));
	CATCH_ERROR_DX(m_Device->CreateRenderTargetView(BackBuffer.Get(), nullptr, &m_RenderTarget));
	BackBuffer.Reset();

	ResizeDepthBuffer(Width, Height);

	m_Context->OMSetRenderTargets(1u, m_RenderTarget.GetAddressOf(), m_DepthTextureView.Get());

	//Setup Viewport
	D3D11_VIEWPORT Viewport;
	Viewport.Width = (FLOAT)Width;
	Viewport.Height = (FLOAT)Height;
	Viewport.MinDepth = 0.0f;
	Viewport.MaxDepth = 1.0f; //Disable depth for testing
	Viewport.TopLeftX = 0.0f;
	Viewport.TopLeftY = 0.0f;

	m_Context->RSSetViewports(1u, &Viewport);

	//VerticalFOV = HorizontalFOV / AspectRatio.
	m_MainCamera->SetProjectionMatrix(DirectX::XMMatrixPerspectiveFovLH(Maths::DegreesToRadians(m_MainCamera->GetFOV()), (float)m_ViewportSize.x / (float)m_ViewportSize.y, m_NearPlane, m_FarPlane));
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
	CATCH_ERROR_DX(m_Device->CreateTexture2D(&DepthDescription, nullptr, &m_DepthTexture));

	// Create the depth stencil view
	D3D11_DEPTH_STENCIL_VIEW_DESC DepthStencilViewDescription = {};
	DepthStencilViewDescription.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	DepthStencilViewDescription.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	DepthStencilViewDescription.Texture2D.MipSlice = 0u;
	CATCH_ERROR_DX(m_Device->CreateDepthStencilView(m_DepthTexture.Get(), &DepthStencilViewDescription, &m_DepthTextureView));
}

void DX11RenderAPI::SetProjectionPerspectiveMatrixDefault()
{
	m_MainCamera->SetProjectionMatrix(DirectX::XMMatrixPerspectiveFovLH(Maths::DegreesToRadians(m_MainCamera->GetFOV()), (float)m_ViewportSize.x / (float)m_ViewportSize.y, m_NearPlane, m_FarPlane));
}

void DX11RenderAPI::SetProjectionOrthographicMatrixDefault()
{
	m_MainCamera->SetProjectionMatrix(DirectX::XMMatrixOrthographicLH((float)m_ViewportSize.x, (float)m_ViewportSize.y, m_NearPlane, m_FarPlane));
}

Vector3 DX11RenderAPI::GetScreenToWorldDirection(int X, int Y, IntVector2 WindowSize, IntVector2 Origin)
{
	//TODO: need to pass in the window size instead incase im checking from any other window
#if WITH_EDITOR
	IntVector2 WinSize = Editor::Get().GetSceneWindowSize();
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
	DirectX::XMStoreFloat4x4(&Projection, m_MainCamera->GetSceneProjectionMatrix());	
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

IntVector2 DX11RenderAPI::WorldToScreen(Vector3 position)
{
	CameraBase* cam = RenderCommand::GetMainCamera();
	Vector3 camPosition = cam->GetCameraPosition();

	DirectX::XMVECTOR vecPosition = DirectX::XMVectorSet(position.x, position.y, position.z, 1.0f);
	IntVector2 WindowSize = GetWindowSize();
	DirectX::XMVECTOR outProjection = DirectX::XMVector3Project(
		vecPosition,
		0.0f, 0.0f,
		(float)WindowSize.x, (float)WindowSize.y,
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
	return m_Device.Get();
}

ID3D11DeviceContext* DX11RenderAPI::GetContext()
{
	return m_Context.Get();
}

void DX11RenderAPI::BindBackBuffer(bool clear)
{
	CREATE_RESULT_HANDLE();

	// Create render target view and populate backbuffer
	Microsoft::WRL::ComPtr<ID3D11Resource> BackBuffer = nullptr;
	CATCH_ERROR_DX(m_SwapChain->GetBuffer(0, __uuidof(ID3D11Resource), &BackBuffer));
	CATCH_ERROR_DX(m_Device->CreateRenderTargetView(BackBuffer.Get(), nullptr, &m_RenderTarget));

	Window& Win = Application::Get().GetWindow();
	IntVector2 AdjWindowSize = dynamic_cast<Window_Win32&>(Win).GetAdjustedWindowSize();
	m_ViewportSize = { AdjWindowSize.x, AdjWindowSize.y };

	//ResizeDepthBuffer(_ViewportSize.X, _ViewportSize.Y);

	m_Context->OMSetRenderTargets(1u, m_RenderTarget.GetAddressOf(), m_DepthTextureView.Get());

	D3D11_VIEWPORT Viewport;
	Viewport.Width = (FLOAT)m_ViewportSize.x;
	Viewport.Height = (FLOAT)m_ViewportSize.y;
	Viewport.MinDepth = 0.0f;
	Viewport.MaxDepth = 1.0f;
	Viewport.TopLeftX = 0.0f;
	Viewport.TopLeftY = 0.0f;

	m_Context->RSSetViewports(1u, &Viewport);

#if WITH_EDITOR
	m_EditorBufferBound = false;
#endif
}

void DX11RenderAPI::BindFrameBuffer(FrameBuffer* Buffer, bool clear)
{
	CREATE_RESULT_HANDLE();

	//Create Render Target view and bind framebuffer
	CATCH_ERROR_DX(m_Device->CreateRenderTargetView(Buffer->GetTexture(), nullptr, &m_RenderTarget));

	m_ViewportSize = { (int)Buffer->GetWidth(), (int)Buffer->GetHeight() };

	if (clear == true)
	{
		if (!Buffer->HasDepthBuffer())
		{
			ResizeDepthBuffer(m_ViewportSize.x, m_ViewportSize.y);
			m_Context->OMSetRenderTargets(1u, m_RenderTarget.GetAddressOf(), m_DepthTextureView.Get());
		}
		else
		{
			m_Context->OMSetRenderTargets(1u, m_RenderTarget.GetAddressOf(), Buffer->GetDepthBuffer()->GetTextureView());

			m_Context->ClearRenderTargetView(m_RenderTarget.Get(), m_BackgroundColour);
			m_Context->ClearDepthStencilView(Buffer->GetDepthBuffer()->GetTextureView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0u);
		}
	}

	D3D11_VIEWPORT Viewport;
	Viewport.Width = (FLOAT)Buffer->GetWidth();
	Viewport.Height = (FLOAT)Buffer->GetHeight();
	Viewport.MinDepth = 0.0f;
	Viewport.MaxDepth = 1.0f;
	Viewport.TopLeftX = 0.0f;
	Viewport.TopLeftY = 0.0f;

	m_Context->RSSetViewports(1u, &Viewport);

	m_CurrentBuffer = Buffer;
#if WITH_EDITOR
	m_EditorBufferBound = false;
#endif
}

void DX11RenderAPI::BindEditorFrameBuffer(bool clear)
{
	//Bind the frame buffer
	BindFrameBuffer(m_EditorBuffer, clear);
	m_EditorBufferBound = true;

	//Update the window rendering properties
	m_MainCamera->SetProjectionMatrix(DirectX::XMMatrixPerspectiveFovLH(Maths::DegreesToRadians(m_MainCamera->GetFOV()), (float)m_ViewportSize.x / (float)m_ViewportSize.y, m_NearPlane, m_FarPlane));
}

FrameBuffer* DX11RenderAPI::GetEditorBuffer() const
{
	return m_EditorBuffer;
}
