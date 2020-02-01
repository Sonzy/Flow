#include "Flowpch.h"
#include "RenderCommand.h"

#include "Flow/Rendering/DX11/DX11RenderAPI.h"

namespace Flow
{
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

	void RenderCommand::SetCameraMatrix(DirectX::FXMMATRIX Matrix)
	{
		dynamic_cast<DX11RenderAPI*>(s_RendererAPI)->SetCameraMatrix(Matrix);
	}

	DirectX::XMMATRIX RenderCommand::GetCameraMatrix()
	{
		return dynamic_cast<DX11RenderAPI*>(s_RendererAPI)->GetCameraMatrix();
	}

	void RenderCommand::SetClearColour(float R, float G, float B, float A)
	{
		s_RendererAPI->SetClearColour(R, G, B, A);
	}

	void RenderCommand::DrawIndexed(int Count)
	{
		s_RendererAPI->DrawIndexed(Count);
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
}

