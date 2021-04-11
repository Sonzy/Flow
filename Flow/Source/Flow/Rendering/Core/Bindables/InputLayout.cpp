// Pch ////////////////////////////////////////////////////////////////////////

#include "pch.h"

// Includes ////////////////////////////////////////////////////////////////////

#include "InputLayout.h"
#include "Framework/Utils/DirectX11/DirectX11Utils.h"
#include "Rendering/Core/Bindables/Codex.h"
#include "Rendering/Renderer.h"

//= Class Definition ///////////////////////////////////////////////////////////

Bindables::InputLayout::InputLayout(VertexLayout Layout, ID3DBlob* vertexShaderByteCode)
	: m_vertexLayout(Layout)
{
	CreateResultHandle();

	const auto D3DLayout = m_vertexLayout.GetD3DLayout();
	CaptureDXError(Renderer::GetDevice()->CreateInputLayout(
		D3DLayout.data(),
		(UINT)D3DLayout.size(),
		vertexShaderByteCode->GetBufferPointer(),
		vertexShaderByteCode->GetBufferSize(),
		&m_inputLayout));
}

void Bindables::InputLayout::Bind()
{
	Renderer::GetContext()->IASetInputLayout(m_inputLayout.Get());
}

Bindables::InputLayout* Bindables::InputLayout::Resolve(const VertexLayout& Layout, ID3DBlob* vertexShaderByteCode)
{
	return Bindables::Codex::Resolve<InputLayout>(Layout, vertexShaderByteCode);
}

HashString Bindables::InputLayout::GenerateID(const VertexLayout& Layout, ID3DBlob* vertexShaderByteCode)
{
	char buffer[64];
	snprintf(buffer, 64, "InputLayout-%s", Layout.GetCode().c_str());
	return buffer;
}

HashString Bindables::InputLayout::GetID()
{
	if (m_id.IsNull())
	{
		m_id = GenerateID(m_vertexLayout);
	}
	return m_id;
}