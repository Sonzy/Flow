#include "PixelShader.h"

namespace Flow
{
	PixelShader::PixelShader(const std::wstring& LocalPath)
	{
		//TODO:	
		HRESULT ResultHandle;

		Microsoft::WRL::ComPtr<ID3DBlob> blob;
		//GFX_THROW_INFO(D3DReadFileToBlob(path.c_str(), &blob));
		//GFX_THROW_INFO(GetDevice(gfx)->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &pixelShader));
	}

	void PixelShader::Bind()
	{
		//TODO: 	GetContext(gfx)->PSSetShader(pixelShader.Get(), nullptr, 0);
	}
}