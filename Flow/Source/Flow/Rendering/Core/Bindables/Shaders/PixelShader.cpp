#include "Flowpch.h"
#include "PixelShader.h"
#include <d3dcompiler.h>

namespace Flow
{
	PixelShader::PixelShader(const std::wstring& LocalPath)
	{
		CREATE_RESULT_HANDLE();

		Microsoft::WRL::ComPtr<ID3DBlob> Blob;
		CATCH_ERROR_DX(D3DReadFileToBlob(LocalPath.c_str(), &Blob));
		CATCH_ERROR_DX(RenderCommand::DX11GetDevice()->CreatePixelShader(Blob->GetBufferPointer(), Blob->GetBufferSize(), nullptr, &m_PixelShader));
	}

	void PixelShader::Bind()
	{
		RenderCommand::DX11GetContext()->PSSetShader(m_PixelShader.Get(), nullptr, 0);
	}
}