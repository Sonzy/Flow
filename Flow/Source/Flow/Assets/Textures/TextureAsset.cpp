#include "Flowpch.h"
#include "TextureAsset.h"
#include "Flow/Assets/AssetSystem.h"
#include "Flow\Application.h"
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
	CATCH_ERROR_DX(DirectX::LoadFromWICFile(WidePath.c_str(), DirectX::WIC_FLAGS_NONE, nullptr, _Image));

	//Convert to our image format
	if (_Image.GetImage(0, 0, 0)->format != _Format)
	{
		DirectX::ScratchImage ConvertedImage;
		CATCH_ERROR_DX(DirectX::Convert(*_Image.GetImage(0, 0, 0), _Format, DirectX::TEX_FILTER_DEFAULT, DirectX::TEX_THRESHOLD_DEFAULT, ConvertedImage));
	}

	//Update asset data
	_AssetSize = sizeof(uint8_t) * _Image.GetPixelsSize();
	_AssetType = EAssetType::Texture;
	_AssetPath = FilePath;

	return true;
}