// Pch ////////////////////////////////////////////////////////////////////////

#include "pch.h"

// Includes ////////////////////////////////////////////////////////////////////

#include <d3d11.h>
#include "Stencil.h"
#include "Rendering/Core/Bindables/Codex.h"
#include "Rendering/Renderer.h"

//= Class (Stencil) Definition ===========

Bindables::Stencil::Stencil(Bindables::Stencil::Mode Mode)
	: m_mode(Mode)
{
	D3D11_DEPTH_STENCIL_DESC Description = CD3D11_DEPTH_STENCIL_DESC{ CD3D11_DEFAULT{} };

	switch (Mode)
	{
	case Bindables::Stencil::Mode::Write:
		Description.DepthEnable = FALSE;
		Description.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
		Description.StencilEnable = TRUE;
		Description.StencilWriteMask = 0xFF;
		Description.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		Description.FrontFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
		break;
	case Bindables::Stencil::Mode::Mask:
		Description.DepthEnable = FALSE;
		Description.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
		Description.StencilEnable = TRUE;
		Description.StencilReadMask = 0xFF;
		Description.FrontFace.StencilFunc = D3D11_COMPARISON_NOT_EQUAL;
		Description.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		break;
	case Bindables::Stencil::Mode::AlwaysOnTop:

		Description.DepthEnable = TRUE;
		Description.DepthFunc = D3D11_COMPARISON_ALWAYS;
		Description.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;

		Description.StencilEnable = TRUE;
		Description.StencilReadMask = 0xFF;

		Description.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		Description.FrontFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
		break;
	case Bindables::Stencil::Mode::NoDepth:
		Description.DepthEnable = FALSE;
		Description.StencilEnable = FALSE;
		break;
	case Bindables::Stencil::Mode::Off:
		break;
	}

	Renderer::GetDevice()->CreateDepthStencilState(&Description, &m_stencil);
}

void Bindables::Stencil::Bind()
{
	Renderer::GetContext()->OMSetDepthStencilState(m_stencil.Get(), 0xFF);
}

Bindables::Stencil* Bindables::Stencil::Resolve(Bindables::Stencil::Mode mode)
{
	return Bindables::Codex::Resolve<Stencil>(mode);
}

HashString Bindables::Stencil::GenerateID(Bindables::Stencil::Mode mode)
{
	char buffer[64];
	snprintf(buffer, 64, "Stencil-%s", Bindables::Stencil::GetModeAsString(mode).c_str());
	return buffer;
}

HashString Bindables::Stencil::GetID()
{
	if (m_id.IsNull())
	{
		m_id = GenerateID(m_mode);
	}
	return m_id;
}

string Bindables::Stencil::GetModeAsString(Bindables::Stencil::Mode mode)
{
	switch (mode)
	{
	case Bindables::Stencil::Mode::Off: 			return "Off"; 
	case Bindables::Stencil::Mode::Write:		return "Write";
	case Bindables::Stencil::Mode::Mask:			return "Mask";
	case Bindables::Stencil::Mode::AlwaysOnTop:	return "AlwaysOnTop";
	}

	return "UNKNOWN";
}
