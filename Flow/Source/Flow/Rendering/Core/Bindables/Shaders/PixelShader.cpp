// Pch ////////////////////////////////////////////////////////////////////////

#include "pch.h"

// Includes ///////////////////////////////////////////////////////////////////

#include <d3dcompiler.h>
#include <d3d11.h>
#include "Framework/Utils/DirectX11/DirectX11Utils.h"
#include "PixelShader.h"
#include "Rendering/Core/Bindables/Codex.h"
#include "Rendering/Renderer.h"

// Class Definition ///////////////////////////////////////////////////////////

Bindables::PixelShader::PixelShader(const std::string& localPath)
	: m_shaderPath(localPath)
{
	CreateResultHandle();

	ID3DBlob* blob;
	std::string temp = localPath.c_str();
	CaptureDXError(D3DReadFileToBlob(std::wstring{ temp.begin(),temp.end() }.c_str(), &blob));
	CaptureDXError(Renderer::GetDevice()->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &m_pixelShader));
}

void Bindables::PixelShader::Bind()
{
	Renderer::GetContext()->PSSetShader(m_pixelShader.Get(), nullptr, 0);
}

Bindables::PixelShader* Bindables::PixelShader::Resolve(const std::string& localPath)
{
	return Bindables::Codex::Resolve<PixelShader>(localPath);
}

HashString Bindables::PixelShader::GenerateID(const std::string& localPath)
{
	char buffer[256];
	snprintf(buffer, 256, "PixelShader-%s", localPath.c_str());
	return buffer;
}

HashString Bindables::PixelShader::GetID()
{
	if (m_id.IsNull())
	{
		m_id = GenerateID(m_shaderPath);
	}
	return m_id;
}