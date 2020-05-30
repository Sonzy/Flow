#include "Flowpch.h"
#include "Stencil.h"

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
	case StencilMode::AlwaysOnTop:

		Description.DepthEnable = TRUE;
		Description.DepthFunc = D3D11_COMPARISON_ALWAYS;
		Description.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;

		Description.StencilEnable = TRUE;
		Description.StencilReadMask = 0xFF;

		Description.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		Description.FrontFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
		break;
	case StencilMode::Off:
		break;
	}

	RenderCommand::DX11GetDevice()->CreateDepthStencilState(&Description, &_Stencil);
}

void Stencil::Bind()
{
	RenderCommand::DX11GetContext()->OMSetDepthStencilState(_Stencil.Get(), 0xFF);
}