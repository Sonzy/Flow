#include "Flowpch.h"
#include "RenderCommand.h"

#include "Flow/Rendering/DX11/DX11RenderAPI.h"

#include "Flow\Application.h"
#include "Flow\GameFramework\World.h"
#include "Flow\GameFramework\Controllers\Controller.h"
#include "Flow/Rendering/Core/Camera/Camera.h"

RenderAPI* RenderCommand::sm_RendererAPI = new DX11RenderAPI();

void RenderCommand::InitialiseDX11(HWND WindowHandle, int ViewportWidth, int ViewportHeight)
{
	sm_RendererAPI->InitialiseDX11API(WindowHandle, ViewportWidth, ViewportHeight);
}

ID3D11Device* RenderCommand::DX11GetDevice()
{
	return dynamic_cast<DX11RenderAPI*>(sm_RendererAPI)->GetDevice();
}

ID3D11DeviceContext* RenderCommand::DX11GetContext()
{
	return dynamic_cast<DX11RenderAPI*>(sm_RendererAPI)->GetContext();
}

RenderAPI::API RenderCommand::GetAPI()
{
	return sm_RendererAPI->GetAPI();
}

CameraBase* RenderCommand::GetMainCamera()
{
	return sm_RendererAPI->GetMainCamera();
}

void RenderCommand::SetMainCamera(CameraBase* NewCamera)
{
	sm_RendererAPI->SetMainCamera(NewCamera);
}

void RenderCommand::Resize(int Width, int Height)
{
	sm_RendererAPI->Resize(Width, Height);
}

void RenderCommand::SetClearColour(float R, float G, float B, float A)
{
	sm_RendererAPI->SetClearColour(R, G, B, A);
}

void RenderCommand::DrawIndexed(int Count)
{
	sm_RendererAPI->DrawIndexed(Count);
}

void RenderCommand::Draw(unsigned int Count)
{
	sm_RendererAPI->Draw(Count);
}

void RenderCommand::ClearWindow()
{
	sm_RendererAPI->Clear();
}
void RenderCommand::BeginFrame()
{
	sm_RendererAPI->BeginFrame();
}
void RenderCommand::EndFrame()
{
	sm_RendererAPI->EndFrame();
}

void RenderCommand::Shutdown()
{
	delete sm_RendererAPI;
}

void RenderCommand::BindEditorBuffer()
{
	dynamic_cast<DX11RenderAPI*>(sm_RendererAPI)->BindEditorFrameBuffer();
}

FrameBuffer* RenderCommand::GetEditorFrameBuffer()
{
	return dynamic_cast<DX11RenderAPI*>(sm_RendererAPI)->GetEditorBuffer();
}

void RenderCommand::BindFrameBuffer(FrameBuffer* Buffer)
{
	sm_RendererAPI->BindFrameBuffer(Buffer);
}

void RenderCommand::BindBackBuffer()
{
	sm_RendererAPI->BindBackBuffer();
}

IntVector2 RenderCommand::GetWindowSize()
{
	return sm_RendererAPI->GetWindowSize();
}

void RenderCommand::SetMinimized(bool Minimized)
{
	sm_RendererAPI->SetWindowMinimized(Minimized);
}

bool RenderCommand::IsMinimized()
{
	return sm_RendererAPI->IsWindowMinimized();
}

Vector3 RenderCommand::GetScreenToWorldDirectionVector(int X, int Y, IntVector2 WindowSize, IntVector2 Origin)
{
	return sm_RendererAPI->GetScreenToWorldDirection(X, Y, WindowSize, Origin);
}