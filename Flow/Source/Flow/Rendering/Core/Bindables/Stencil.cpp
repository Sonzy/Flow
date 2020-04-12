#include "Flowpch.h"
#include "Stencil.h"

namespace Flow
{
	Stencil::Stencil(StencilMode Mode)
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
			Description.DepthEnable = FALSE;
			Description.StencilEnable = TRUE;
			Description.StencilReadMask = 0xFF;
			Description.FrontFace.StencilFunc = D3D11_COMPARISON_NOT_EQUAL;
			Description.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
			break;
		case StencilMode::Off:
			FLOW_ENGINE_WARNING("Stencil::Stencil: Created a stencil with the mode StencilMode::Off");
			break;
		}
		
		RenderCommand::DX11GetDevice()->CreateDepthStencilState(&Description, &Stencil_);
	}

	void Stencil::Bind()
	{
		RenderCommand::DX11GetContext()->OMSetDepthStencilState(Stencil_.Get(), 0xFF);
	}
}