#include "Flowpch.h"
#include "Texture.h"
#include "Flow/ErrorHandling/ErrorMacros.h"

namespace Flow
{
	Texture::Texture(TextureAsset* Asset)
	{
		HRESULT ResultHandle;

		//Create the texture resource
		D3D11_TEXTURE2D_DESC textureDesc = {};
		textureDesc.Width = Asset->GetWidth();
		textureDesc.Height = Asset->GetHeight();
		textureDesc.MipLevels = 1;
		textureDesc.ArraySize = 1;
		textureDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		textureDesc.CPUAccessFlags = 0;
		textureDesc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA sd = {};
		sd.pSysMem = Asset->GetBufferPtr();
		sd.SysMemPitch = Asset->GetWidth() * sizeof(TexColor);

		Microsoft::WRL::ComPtr<ID3D11Texture2D> pTexture;
		CATCH_ERROR_DX(RenderCommand::DX11GetDevice()->CreateTexture2D(&textureDesc, &sd, &pTexture));

		//Create a view to the texture
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Format = textureDesc.Format;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MostDetailedMip = 0;
		srvDesc.Texture2D.MipLevels = 1;
		CATCH_ERROR_DX(RenderCommand::DX11GetDevice()->CreateShaderResourceView(pTexture.Get(), &srvDesc, &m_TextureView));
	}

	void Texture::Bind()
	{
		RenderCommand::DX11GetContext()->PSSetShaderResources(0u, 1u, m_TextureView.GetAddressOf());
	}
}

