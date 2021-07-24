// Pch ////////////////////////////////////////////////////////////////////////

#include "pch.h"

// Includes ///////////////////////////////////////////////////////////////////

#include <d3d11.h>
#include <d3dcompiler.h>
#include "VertexShader.h"
#include "Framework/Utils/DirectX11/DirectX11Utils.h"
#include "Rendering/Core/Bindables/Codex.h"
#include "Rendering/Renderer.h"

// Class Definition ///////////////////////////////////////////////////////////

Bindables::VertexShader::VertexShader(const std::string& LocalPath)
	: m_shaderPath(LocalPath)
{
	CreateResultHandle();

	std::string temp = LocalPath.c_str();
	CaptureDXError(D3DReadFileToBlob(std::wstring{ temp.begin(),temp.end() }.c_str(), &m_blob));
	CaptureDXError(Renderer::GetDevice()->CreateVertexShader(m_blob->GetBufferPointer(), m_blob->GetBufferSize(), nullptr, &m_vertexShader));
}

void Bindables::VertexShader::Bind()
{
	Renderer::GetContext()->VSSetShader(m_vertexShader.Get(), nullptr, 0);
}

ID3DBlob* Bindables::VertexShader::GetByteCode() const
{
	return m_blob;
}

Bindables::VertexShader* Bindables::VertexShader::Resolve(const std::string& LocalPath)
{
	return Bindables::Codex::Resolve<VertexShader>(LocalPath);
}

HashString Bindables::VertexShader::GenerateID(const std::string& LocalPath)
{
	char buffer[256];
	snprintf(buffer, 256, "VertexShader-%s", LocalPath.c_str());
	return buffer;
}

HashString Bindables::VertexShader::GetID()
{
	if (m_id.IsNull())
	{
		m_id = GenerateID(m_shaderPath);
	}
	return m_id;
}

