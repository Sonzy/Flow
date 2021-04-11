// Pch ////////////////////////////////////////////////////////////////

#include "pch.h"

// Includes ///////////////////////////////////////////////////////////

#include <d3d11.h>
#include "BlendState.h"
#include "Framework/Utils/DirectX11/DirectX11Utils.h"
#include "Rendering/Core/Bindables/Codex.h"
#include "Rendering/Renderer.h"

// Class Definition ///////////////////////////////////////////////////

BlendState::BlendState(bool enabled)
	: m_enabled(enabled)
{
	D3D11_BLEND_DESC blendDesc = {};
	auto& rtDesc = blendDesc.RenderTarget[0];

	if (m_enabled)
	{
		rtDesc.BlendEnable = TRUE;
		rtDesc.SrcBlend = D3D11_BLEND_SRC_ALPHA;
		rtDesc.DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		rtDesc.BlendOp = D3D11_BLEND_OP_ADD;
		rtDesc.SrcBlendAlpha = D3D11_BLEND_ZERO;
		rtDesc.DestBlendAlpha = D3D11_BLEND_ZERO;
		rtDesc.BlendOpAlpha = D3D11_BLEND_OP_ADD;
		rtDesc.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	}
	else
	{
		rtDesc.BlendEnable = FALSE;
		rtDesc.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	}

	CreateResultHandle();
	CaptureDXError(Renderer::GetDevice()->CreateBlendState(&blendDesc, &m_blendState));
}

BlendState* BlendState::Resolve(bool enabled)
{
	return Bindables::Codex::Resolve<BlendState>(enabled);
}

void BlendState::Bind()
{
	Renderer::GetContext()->OMSetBlendState(m_blendState.Get(), nullptr, 0xFFFFFFFFu);
}

HashString BlendState::GenerateID(bool enabled)
{
	char buffer[64];
	snprintf(buffer, 64, "BlendState-%s", enabled ? "enabled" : "disabled");
	return buffer;
}

HashString BlendState::GetID()
{
	if (m_id.IsNull())
	{
		m_id = GenerateID(m_enabled);
	}
	return m_id;
}
