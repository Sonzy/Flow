//= Includes =============================================

#include "pch.h"
#include "Texture.h"
#include "Flow/ErrorHandling/ErrorMacros.h"
#include "BindableCodex.h"
#include "Flow\Assets\AssetSystem.h"

//= Class (Texture) Definition ===========================

Texture::Texture(TextureAsset* Asset, UINT Slot)
{
	HRESULT ResultHandle;

	// Bindable context stuff
	m_AssetName = Asset->GetAssetName();
	m_Slot = Slot;

	//Create the texture resource
	D3D11_TEXTURE2D_DESC textureDesc = {};
	textureDesc.Width = Asset->GetWidth();
	textureDesc.Height = Asset->GetHeight();
	textureDesc.MipLevels = 0; //Use all mips
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

	//Disabled for mip mapping
	//D3D11_SUBRESOURCE_DATA sd = {};
	//sd.pSysMem = Asset->GetBufferPtr();
	//sd.SysMemPitch = Asset->GetPitch();

	Microsoft::WRL::ComPtr<ID3D11Texture2D> pTexture;
	CATCH_ERROR_DX(RenderCommand::DX11GetDevice()->CreateTexture2D(&textureDesc, nullptr, &pTexture));

	RenderCommand::DX11GetContext()->UpdateSubresource(pTexture.Get(), 0u, nullptr, Asset->GetBufferPtr(), Asset->GetPitch(), 0u);

	//Create a view to the texture
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = textureDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = -1;
	CATCH_ERROR_DX(RenderCommand::DX11GetDevice()->CreateShaderResourceView(pTexture.Get(), &srvDesc, &m_TextureView));

	RenderCommand::DX11GetContext()->GenerateMips(m_TextureView.Get());
}

Texture::Texture(const DirectX::ScratchImage& Asset, UINT Slot, const std::string& AssetName)
{
	HRESULT ResultHandle;

	// Bindable context stuff
	m_AssetName = AssetName;
	m_Slot = Slot;

	//Create the texture resource
	D3D11_TEXTURE2D_DESC textureDesc = {};
	textureDesc.Width = static_cast<unsigned int>(Asset.GetMetadata().width);
	textureDesc.Height = static_cast<unsigned int>(Asset.GetMetadata().height);
	textureDesc.MipLevels = 0; //Use all mips
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

	//Disabled for mip mapping
	//D3D11_SUBRESOURCE_DATA sd = {};
	//sd.pSysMem = Asset->GetBufferPtr();
	//sd.SysMemPitch = Asset->GetPitch();

	Microsoft::WRL::ComPtr<ID3D11Texture2D> pTexture;
	CATCH_ERROR_DX(RenderCommand::DX11GetDevice()->CreateTexture2D(&textureDesc, nullptr, &pTexture));

	RenderCommand::DX11GetContext()->UpdateSubresource(pTexture.Get(), 0u, nullptr, Asset.GetPixels(), static_cast<UINT>(Asset.GetImage(0, 0, 0)->rowPitch), 0u);

	//Create a view to the texture
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = textureDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = -1;
	CATCH_ERROR_DX(RenderCommand::DX11GetDevice()->CreateShaderResourceView(pTexture.Get(), &srvDesc, &m_TextureView));

	RenderCommand::DX11GetContext()->GenerateMips(m_TextureView.Get());
}

void Texture::Bind()
{
	RenderCommand::DX11GetContext()->PSSetShaderResources(m_Slot, 1u, m_TextureView.GetAddressOf());
}
Bindable* Texture::Resolve(TextureAsset* Asset, UINT Slot)
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
	return GenerateUID(AssetSystem::GetAsset<TextureAsset>(m_AssetName), m_Slot);
}