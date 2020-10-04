//= Includes =============================

#include "Flowpch.h"
#include "Stencil.h"
#include "BindableCodex.h"

//= Class (Stencil) Definition ===========

Stencil::Stencil(StencilMode Mode)
	: m_Mode(Mode)
{
	D3D11_DEPTH_STENCIL_DESC Description = CD3D11_DEPTH_STENCIL_DESC{ CD3D11_DEFAULT{} };

	switch (Mode)
	{
	case StencilMode::Write:
		Description.DepthEnable = FALSE;
		Description.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
		Description.StencilEnable = TRUE;
		Description.StencilWriteMask = 0xFF;
		Description.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		Description.FrontFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
		break;
	case StencilMode::Mask:
		Description.DepthEnable = FALSE;
		Description.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
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

	RenderCommand::DX11GetDevice()->CreateDepthStencilState(&Description, &m_Stencil);
}

void Stencil::Bind()
{
	RenderCommand::DX11GetContext()->OMSetDepthStencilState(m_Stencil.Get(), 0xFF);
}

std::shared_ptr<Bindable> Stencil::Resolve(StencilMode mode)
{
	return BindableCodex::Resolve<Stencil>(mode);
}

std::string Stencil::GenerateUID(StencilMode mode)
{
	using namespace std::string_literals;
	return typeid(Stencil).name() + "#"s + Stencil::GetModeAsString(mode);
}

std::string Stencil::GetUID() const
{
	return GenerateUID(m_Mode);
}

std::string Stencil::GetModeAsString(StencilMode mode)
{
	switch (mode)
	{
	case StencilMode::Off: 			return "Off"; 
	case StencilMode::Write:		return "Write";
	case StencilMode::Mask:			return "Mask";
	case StencilMode::AlwaysOnTop:	return "AlwaysOnTop";
	}

	return "UNKNOWN";
}
