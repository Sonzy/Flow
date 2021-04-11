// Pch ////////////////////////////////////////////////////////////////////////

#include "pch.h"

// Includes ///////////////////////////////////////////////////////////////////

#include <d3d11.h>
#include "Assets/Textures/TextureAsset.h"
#include "Framework/Utils/DirectX11/DirectX11Utils.h"
#include "Rendering/Renderer.h"
#include "Rendering/Core/Bindables/Codex.h"
#include "Texture.h"

// Class Definition ///////////////////////////////////////////////////////////

Bindables::Texture::Texture(TextureAsset* asset, UINT slot)
	: m_asset(asset)
{
	CreateResultHandle();

	// Bindable context stuff
	m_slot = slot;

	//Create the texture resource
	D3D11_TEXTURE2D_DESC textureDesc = {};
	textureDesc.Width = asset->GetWidth();
	textureDesc.Height = asset->GetHeight();
	textureDesc.MipLevels = 0; //Use all mips
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

	CaptureDXError(Renderer::GetDevice()->CreateTexture2D(&textureDesc, nullptr, &m_texture));

	Renderer::GetContext()->UpdateSubresource(m_texture.Get(), 0u, nullptr, asset->GetBufferPtr(), asset->GetPitch(), 0u);

	//Create a view to the texture
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = textureDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = -1;
	CaptureDXError(Renderer::GetDevice()->CreateShaderResourceView(m_texture.Get(), &srvDesc, &m_textureView));

	Renderer::GetContext()->GenerateMips(m_textureView.Get());
}

Bindables::Texture::Texture(const DirectX::ScratchImage& Asset, UINT slot, const std::string& AssetName)
	: m_slot(slot)
	, m_asset(nullptr)
	//, m_customName(AssetName.c_str())
{
	HRESULT ResultHandle;

	// Bindable context stuff
	m_slot = slot;

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

	CaptureDXError(Renderer::GetDevice()->CreateTexture2D(&textureDesc, nullptr, &m_texture));

	Renderer::GetContext()->UpdateSubresource(m_texture.Get(), 0u, nullptr, Asset.GetPixels(), static_cast<UINT>(Asset.GetImage(0, 0, 0)->rowPitch), 0u);

	//Create a view to the texture
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = textureDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = -1;
	CaptureDXError(Renderer::GetDevice()->CreateShaderResourceView(m_texture.Get(), &srvDesc, &m_textureView));

	Renderer::GetContext()->GenerateMips(m_textureView.Get());
}

Bindables::Texture::~Texture()
{
	m_textureView.Reset();
	m_texture.Reset();
}

void Bindables::Texture::Bind()
{
	Renderer::GetContext()->PSSetShaderResources(m_slot, 1u, m_textureView.GetAddressOf());
}

ID3D11ShaderResourceView* Bindables::Texture::GetTextureView() const
{
	return m_textureView.Get();
}

Bindables::Texture* Bindables::Texture::Resolve(TextureAsset* asset, UINT slot)
{
	return Bindables::Codex::Resolve<Texture>(asset, slot);
}

HashString Bindables::Texture::GenerateID(TextureAsset* asset, UINT slot)
{
	char buffer[256];
	snprintf(buffer, 256, "Texture-%s_%d", asset->GetAssetPath().c_str(), slot);
	return buffer;
}

HashString Bindables::Texture::GetID()
{
	if (m_id.IsNull())
	{
		m_id = GenerateID(m_asset, m_slot);
	}
	return m_id;
}