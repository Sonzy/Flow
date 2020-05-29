#include "Flowpch.h"
#include "RenderCommand.h"

#include "Flow/Rendering/DX11/DX11RenderAPI.h"

#include "Flow\Application.h"
#include "Flow\GameFramework\World.h"
#include "Flow\GameFramework\Controllers\Controller.h"
#include "Flow/Rendering/Core/Camera/Camera.h"

RenderAPI* RenderCommand::s_RendererAPI = new DX11RenderAPI();

void RenderCommand::InitialiseDX11(HWND WindowHandle, int ViewportWidth, int ViewportHeight)
{
	s_RendererAPI->InitialiseDX11API(WindowHandle, ViewportWidth, ViewportHeight);
}

ID3D11Device* RenderCommand::DX11GetDevice()
{
	return dynamic_cast<DX11RenderAPI*>(s_RendererAPI)->GetDevice();
}

ID3D11DeviceContext* RenderCommand::DX11GetContext()
{
	return dynamic_cast<DX11RenderAPI*>(s_RendererAPI)->GetContext();
}

std::shared_ptr<CameraBase> RenderCommand::GetMainCamera()
{
	return s_RendererAPI->GetMainCamera();
		//Application::GetWorld()->GetLocalController()->GetCamera();
}

void RenderCommand::SetMainCamera(std::shared_ptr<CameraBase> NewCamera)
{
	s_RendererAPI->SetMainCamera(NewCamera);
}

void RenderCommand::Resize(int Width, int Height)
{
	s_RendererAPI->Resize(Width, Height);
}

void RenderCommand::SetClearColour(float R, float G, float B, float A)
{
	s_RendererAPI->SetClearColour(R, G, B, A);
}

void RenderCommand::DrawIndexed(int Count)
{
	s_RendererAPI->DrawIndexed(Count);
}

void RenderCommand::Draw(unsigned int Count)
{
	s_RendererAPI->Draw(Count);
}

void RenderCommand::ClearWindow()
{
	s_RendererAPI->Clear();
}
void RenderCommand::BeginFrame()
{
	s_RendererAPI->BeginFrame();
}
void RenderCommand::EndFrame()
{
	s_RendererAPI->EndFrame();
}

void RenderCommand::Shutdown()
{
	delete s_RendererAPI;
}

IntVector2D RenderCommand::GetWindowSize()
{
	return s_RendererAPI->GetWindowSize();
}

Vector RenderCommand::GetScreenToWorldDirectionVector(int X, int Y)
{
	return s_RendererAPI->GetScreenToWorldDirection(X, Y);
}