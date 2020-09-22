#include "Flowpch.h"
#include "TextureAsset.h"
#include "DXTex/DirectXTex.h"

TextureAsset::~TextureAsset()
{
}

bool TextureAsset::LoadAsset(const std::string& FilePath)
{
	HRESULT ResultHandle;

	//Load file using DirectXTex
	std::string Path = FilePath;
	std::wstring WidePath = std::wstring(Path.begin(), Path.end());
	CATCH_ERROR_DX(DirectX::LoadFromWICFile(WidePath.c_str(), DirectX::WIC_FLAGS_NONE, nullptr, m_Image));

	//Convert to our image format
	DXGI_FORMAT ImageFormat = m_Image.GetImage(0, 0, 0)->format;
	if (ImageFormat != m_Format)
	{
		DirectX::ScratchImage ConvertedImage;
		CATCH_ERROR_DX(DirectX::Convert(*m_Image.GetImage(0, 0, 0), m_Format, DirectX::TEX_FILTER_DEFAULT, DirectX::TEX_THRESHOLD_DEFAULT, ConvertedImage));

		m_Image.InitializeFromImage(*ConvertedImage.GetImage(0, 0, 0));
	}

	//Update asset data
	m_AssetSize = sizeof(uint8_t) * m_Image.GetPixelsSize();
	m_AssetType = Asset::Type::Texture;
	m_AssetPath = FilePath;

	return true;
}