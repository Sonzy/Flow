// Pch ////////////////////////////////////////////////////////////////////////

#include "pch.h"

// Includes ///////////////////////////////////////////////////////////////////

#include "pch.h"
#include <d3d11.h>
#include "Framework/Utils/DirectX11/DirectX11Utils.h"
#include "Rasterizer.h"
#include "Rendering/Core/Bindables/Codex.h"
#include "Rendering/Renderer.h"

// Class Definition ///////////////////////////////////////////////////////////

Rasterizer::Rasterizer(CullMode mode)
	: m_cullMode(mode)
{
	CreateResultHandle();

	CD3D11_RASTERIZER_DESC Description = CD3D11_RASTERIZER_DESC(CD3D11_DEFAULT{});
	Description.CullMode = static_cast<D3D11_CULL_MODE>(m_cullMode); 

	//TODO: Need to flip the normals in the shaders if it is a back face
	CaptureDXError(Renderer::GetDevice()->CreateRasterizerState(&Description, &m_rasterizerState));
}

void Rasterizer::Bind()
{
	Renderer::GetContext()->RSSetState(m_rasterizerState.Get());
}

HashString Rasterizer::GetID()
{
	if (m_id.IsNull())
	{
		m_id = GenerateID(m_cullMode);
	}
	return m_id;
}

Rasterizer* Rasterizer::Resolve(CullMode mode)
{
	return Bindables::Codex::Resolve<Rasterizer>(mode);
}

HashString Rasterizer::GenerateID(CullMode mode)
{
	char buffer[64];
	snprintf(buffer, 32, "Rasterizer-%s", CullModeToString(mode));
	return buffer;
}

const char* Rasterizer::CullModeToString(CullMode mode)
{
	switch (mode)
	{
	case Rasterizer::Cull_None:		return "None";
	case Rasterizer::Cull_Back:		return "Back";
	case Rasterizer::Cull_Front:	return "Front";
	default:						return "Unknown";
	}
}
