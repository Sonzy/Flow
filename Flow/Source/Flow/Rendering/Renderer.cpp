// Pch //////////////////////////////////////////////////////////////

#include "pch.h"

// Includes /////////////////////////////////////////////////////////

#include "Renderer.h"
#include "DX11/DX11RenderAPI.h"
#include "GameFramework/Components/RenderableComponent.h"
#include "Rendering/Core/RenderQueue/Technique.h"
#include "Rendering/Core/RenderQueue/RenderQueue.h"

// Static Variable Definitions //////////////////////////////////////

DX11RenderAPI* Renderer::sm_renderApi = new DX11RenderAPI();
CameraBase* Renderer::sm_mainCamera = nullptr;
int Renderer::s_objectsRendered = 0;

// Function Definitions /////////////////////////////////////////////

void Renderer::BeginScene()
{
	s_objectsRendered = 0;

#if WITH_EDITOR
	Renderer::BindGameBuffer();
#else
	Renderer::BindBackBuffer();
#endif
}

void Renderer::EndScene()
{
	RenderQueue::Execute();
	RenderQueue::Reset();

	// We draw the Editor to the back buffer directly.
#if WITH_EDITOR
	Renderer::BindBackBuffer();
#endif
}

int Renderer::SubmitWithoutDraw(Renderable* Renderables)
{
	FLOW_ENGINE_WARNING("Renderer::SubmitWithoutDraw: TODO: Update");
	return 0;
}

void Renderer::Submit(Renderable* const  Object)
{
	PROFILE_FUNCTION();

	CHECK_RETURN(!Object, "Renderer::Submit: Renderable was nullptr");

	assert(Object->m_techniques.size() != 0 && "Renderer::Submit: Object %s had 0 techniques");

	for (const auto& T : Object->m_techniques)
	{
		T.Submit(*Object);
	}

	s_objectsRendered++;
}

void Renderer::Draw(int Count)
{
	Renderer::DrawIndexed(Count);
}

void Renderer::Initialise(HWND WindowHandle, int ViewportWidth, int ViewportHeight)
{
	sm_renderApi->Initialise(WindowHandle, ViewportWidth, ViewportHeight);
}

void Renderer::Shutdown()
{
	delete sm_renderApi;
}

ID3D11Device* Renderer::GetDevice()
{
	return sm_renderApi->GetDevice();
}

ID3D11DeviceContext* Renderer::GetContext()
{
	return sm_renderApi->GetContext();
}

CameraBase* Renderer::GetMainCamera()
{
	return sm_mainCamera;
}

void Renderer::SetMainCamera(CameraBase* NewCamera)
{
	sm_mainCamera = NewCamera;
}

void Renderer::SetOrthographic()
{
	sm_renderApi->SetProjectionOrthographicMatrixDefault();
}

void Renderer::SetPerspective()
{
	sm_renderApi->SetProjectionPerspectiveMatrixDefault();
}

void Renderer::SetNearPlane(float dist)
{
	sm_renderApi->SetNearPlane(dist);
}

void Renderer::SetFarPlane(float dist)
{
	sm_renderApi->SetFarPlane(dist);
}

float& Renderer::GetNearPlaneRef()
{
	return sm_renderApi->GetNearPlaneRef();
}

float& Renderer::GetFarPlaneRef()
{
	return sm_renderApi->GetFarPlaneRef();
}

void Renderer::Resize(int width, int height)
{
	return sm_renderApi->Resize(width, height);
}

void Renderer::ClearWindow()
{
	sm_renderApi->Clear();
}

void Renderer::SetClearColour(float R, float G, float B, float A)
{
	sm_renderApi->SetClearColour(R, G, B, A);
}

IntVector2 Renderer::GetWindowSize()
{
	return sm_renderApi->GetWindowSize();
}

void Renderer::SetMinimized(bool minimized)
{
	sm_renderApi->SetWindowMinimized(minimized);
}

bool Renderer::IsMinimized()
{
	return sm_renderApi->IsWindowMinimized();
}

void Renderer::DrawIndexed(int Count)
{
	sm_renderApi->DrawIndexed(Count);
}

void Renderer::Draw(unsigned int Count)
{
	sm_renderApi->Draw(Count);
}

void Renderer::BeginFrame()
{
	sm_renderApi->BeginFrame();
}

void Renderer::EndFrame()
{
	sm_renderApi->EndFrame();
}

#if WITH_EDITOR
void Renderer::BindGameBuffer()
{
	sm_renderApi->BindGameFrameBuffer(true);
}

void Renderer::BindGameBufferWithoutClear()
{
	sm_renderApi->BindGameFrameBuffer(false);
}

FrameBuffer* Renderer::GetGameFrameBuffer()
{
	return sm_renderApi->GetGameBuffer();
}
#endif // WITH_EDITOR

void Renderer::BindFrameBuffer(FrameBuffer* Buffer)
{
	sm_renderApi->BindFrameBuffer(Buffer);
}

void Renderer::BindBackBuffer()
{
	sm_renderApi->BindBackBuffer();
}

void Renderer::BindFrameBufferWithoutClear(FrameBuffer* Buffer)
{
	sm_renderApi->BindFrameBuffer(Buffer, false);
}

void Renderer::BindBackBufferWithoutClear()
{
	sm_renderApi->BindBackBuffer(false);
}

Vector3 Renderer::GetScreenToWorldDirectionVector(int X, int Y, IntVector2 WindowSize, IntVector2 Origin)
{
	return sm_renderApi->GetScreenToWorldDirection(X, Y, WindowSize, Origin);
}

IntVector2 Renderer::WorldToScreen(Vector3 position)
{
	return sm_renderApi->WorldToScreen(position);
}

RenderPass Renderer::GetActivePass()
{
	return RenderQueue::GetActiveRenderPass();
}
