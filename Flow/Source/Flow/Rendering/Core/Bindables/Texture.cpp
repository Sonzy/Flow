#include "Flowpch.h"
#include "Texture.h"
#include "Flow/ErrorHandling/ErrorMacros.h"
#include "BindableCodex.h"
#include "Flow\Assets\AssetSystem.h"

Texture::Texture(TextureAsset* Asset, UINT Slot)
{
	HRESULT ResultHandle;

	// Bindable context stuff
	_AssetName = Asset->GetAssetName();
	_Slot = Slot;

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
	sd.SysMemPitch = Asset->GetPitch();

	Microsoft::WRL::ComPtr<ID3D11Texture2D> pTexture;
	CATCH_ERROR_DX(RenderCommand::DX11GetDevice()->CreateTexture2D(&textureDesc, &sd, &pTexture));

	//Create a view to the texture
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = textureDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = 1;
	CATCH_ERROR_DX(RenderCommand::DX11GetDevice()->CreateShaderResourceView(pTexture.Get(), &srvDesc, &_TextureView));
}

void Texture::Bind()
{
	RenderCommand::DX11GetContext()->PSSetShaderResources(_Slot, 1u, _TextureView.GetAddressOf());
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
	return GenerateUID(AssetSystem::GetAsset<TextureAsset>(_AssetName), _Slot);
}