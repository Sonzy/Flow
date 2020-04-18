#include "Flowpch.h"
#include "Stencil.h"

namespace Flow
{
	Stencil::Stencil(StencilMode Mode, DepthMode Depth)
	{
		D3D11_DEPTH_STENCIL_DESC Description = CD3D11_DEPTH_STENCIL_DESC{ CD3D11_DEFAULT{} };

		switch (Mode)
		{
		case StencilMode::Write:
			Description.StencilEnable = TRUE;
			Description.StencilWriteMask = 0xFF;
			Description.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
			Description.FrontFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
			break;
		case StencilMode::Mask:
			Description.StencilEnable = TRUE;
			Description.StencilReadMask = 0xFF;
			Description.FrontFace.StencilFunc = D3D11_COMPARISON_NOT_EQUAL;
			Description.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
			break;
		case StencilMode::Off:
			//Description.StencilEnable = FALSE;
			break;
		}

		switch (Depth)
		{
		case Flow::DepthMode::Off:
			Description.DepthEnable = FALSE;
			break;
		case Flow::DepthMode::On:
			Description.DepthEnable = TRUE;
			break;
		}
		
		RenderCommand::DX11GetDevice()->CreateDepthStencilState(&Description, &Stencil_);
	}

	void Stencil::Bind()
	{
		RenderCommand::DX11GetContext()->OMSetDepthStencilState(Stencil_.Get(), 0xFF);
	}
}