#include "Flowpch.h"
#include "TextureAsset.h"

#include "Flow\Application.h"
#include "DXTex/DirectXTex.h"

namespace Flow
{
	TextureAsset::~TextureAsset()
	{
	}

	bool TextureAsset::LoadAsset(const std::string& FilePath)
	{
		HRESULT ResultHandle;

		//Load file using DirectXTex
		std::string Path = Application::GetApplication().GetLocalFilePath() + FilePath;
		std::wstring WidePath = std::wstring(Path.begin(), Path.end());
		CATCH_ERROR_DX(DirectX::LoadFromWICFile(WidePath.c_str(), DirectX::WIC_FLAGS_NONE, nullptr, Image_));

		//Convert to our image format
		if (Image_.GetImage(0, 0, 0)->format != Format_)
		{
			DirectX::ScratchImage ConvertedImage;
			CATCH_ERROR_DX(DirectX::Convert(*Image_.GetImage(0, 0, 0), Format_, DirectX::TEX_FILTER_DEFAULT, DirectX::TEX_THRESHOLD_DEFAULT, ConvertedImage));
		}

		//Update asset data
		AssetSize_ = sizeof(uint8_t) * Image_.GetPixelsSize();
		AssetType_ = EAssetType::Texture;
		m_AssetPath = FilePath;

		return true;
	}
}

