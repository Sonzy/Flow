#include "Flowpch.h"
#include "VertexShader.h"
#include <d3dcompiler.h>

namespace Flow
{
	VertexShader::VertexShader(const std::wstring& LocalPath)
	{
		CREATE_RESULT_HANDLE();

		CATCH_ERROR_DX(D3DReadFileToBlob(LocalPath.c_str(), &m_Blob));
		CATCH_ERROR_DX(RenderCommand::DX11GetDevice()->CreateVertexShader(m_Blob->GetBufferPointer(), m_Blob->GetBufferSize(), nullptr, &m_VertexShader));
	}

	void VertexShader::Bind()
	{
		RenderCommand::DX11GetContext()->VSSetShader(m_VertexShader.Get(), nullptr, 0);
	}

	ID3DBlob* VertexShader::GetByteCode() const
	{
		return m_Blob.Get();
	}
}

