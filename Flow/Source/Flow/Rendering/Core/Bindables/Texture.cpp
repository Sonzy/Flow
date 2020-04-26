#include "Flowpch.h"
#include "Texture.h"
#include "Flow/ErrorHandling/ErrorMacros.h"
#include "BindableCodex.h"
#include "Flow\Assets\AssetSystem.h"

namespace Flow
{
	Texture::Texture(TextureAsset* Asset, UINT Slot)
	{
		HRESULT ResultHandle;

		// Bindable context stuff
		AssetName_ = Asset->GetAssetName();
		Slot_ = Slot;

		//Create the texture resource
		D3D11_TEXTURE2D_DESC textureDesc = {};
		textureDesc.Width = Asset->GetWidth();
		textureDesc.Height = Asset->GetHeight();
		textureDesc.MipLevels = 1;
		textureDesc.ArraySize = 1;
		textureDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
		//textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.Usage = D3D11_USAGE_DYNAMIC;
		textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		//textureDesc.CPUAccessFlags = 0;
		textureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		textureDesc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA sd = {};
		sd.pSysMem = Asset->GetBufferPtr();
		sd.SysMemPitch = Asset->GetPitch();

		CATCH_ERROR_DX(RenderCommand::DX11GetDevice()->CreateTexture2D(&textureDesc, &sd, &pTexture));

		//Create a view to the texture
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Format = textureDesc.Format;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MostDetailedMip = 0;
		srvDesc.Texture2D.MipLevels = 1;
		CATCH_ERROR_DX(RenderCommand::DX11GetDevice()->CreateShaderResourceView(pTexture.Get(), &srvDesc, &TextureView_));
	}

	Texture::~Texture()
	{
		TextureView_.Reset();
		pTexture.Reset();
	}

	void Texture::Bind()
 	{
		RenderCommand::DX11GetContext()->PSSetShaderResources(Slot_, 1u, TextureView_.GetAddressOf());
	}

	void Texture::Update(TextureAsset* Tex)
	{
		PROFILE_FUNCTION();
		
		D3D11_MAPPED_SUBRESOURCE MappedRes;
		RenderCommand::DX11GetContext()->Map(pTexture.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedRes);

		BYTE* MappedData = reinterpret_cast<BYTE*>(MappedRes.pData);
		unsigned int CurrentPitch;
		uint8_t* bufferptr = Tex->GetBufferPtr();
		for (UINT i = 0; i < Tex->GetHeight(); i++)
		{
			memcpy(MappedData, bufferptr, Tex->GetPitch());
			MappedData += MappedRes.RowPitch;
			bufferptr += Tex->GetPitch();			
		}

		RenderCommand::DX11GetContext()->Unmap(pTexture.Get(), 0);
	}

	std::shared_ptr<Bindable> Texture::Resolve(TextureAsset* Asset, UINT Slot)
	{
		return BindableCodex::Resolve<Texture>(Asset, Slot);
	}
	std::string Texture::GenerateUID(TextureAsset* Asset, UINT Slot)
	{
		using namespace std::string_literals; //Using literal operator to force these to strings
		return typeid(Texture).name() + "#"s + Asset->GetAssetPath() + "#"s + std::to_string(Slot);
	}
	std::string Texture::GetUID() const
	{
		return GenerateUID(AssetSystem::GetAsset<TextureAsset>(AssetName_), Slot_);
	}
}

