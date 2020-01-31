#include "VertexShader.h"

namespace Flow
{
	VertexShader::VertexShader(const std::wstring& LocalPath)
	{
		//TODO
	}

	void VertexShader::Bind()
	{
		//TODO
	}

	ID3DBlob* VertexShader::GetByteCode() const
	{
		return m_Blob.Get();
	}
}

