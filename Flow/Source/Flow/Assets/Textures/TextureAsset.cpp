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

	void TextureAsset::ManualInit(size_t Width, size_t Height)
	{
		CREATE_RESULT_HANDLE();
		CATCH_ERROR_DX( Image_.Initialize2D(Format_, Width, Height, 1u, 1u));
	}

	void TextureAsset::AlterPixel(unsigned int x, unsigned int y, TexColor Colour)
	{
		PROFILE_FUNCTION();

		CHECK_RETURN(x < 0, ("TextureAsset::AlterPixel: X Was less than 0 ( {0} )", x));
		CHECK_RETURN(y < 0, ("TextureAsset::AlterPixel: Y was less than 0 ( {0} )", y));
		CHECK_RETURN(x >= GetWidth(), ("TextureAsset::AlterPixel: X was more than width (X: {0}) (Width: {1})", x, GetWidth()));
		CHECK_RETURN(y >= GetHeight(), ("TextureAsset::AlterPixel: Y was more than height(X : {0}) (Height : {1})", y, GetHeight()));

		auto& ImageData = *Image_.GetImage(0, 0, 0);
		reinterpret_cast<TexColor*>(&ImageData.pixels[y * ImageData.rowPitch])[x] = Colour;
	}

	void TextureAsset::OverwriteAssetPath(const std::string& NewPath)
	{
		m_AssetPath = NewPath;
	}

	unsigned int TextureAsset::GetWidth() const
	{
		return (unsigned int)Image_.GetMetadata().width;
	}
	unsigned int TextureAsset::GetHeight() const
	{
		return (unsigned int)Image_.GetMetadata().height;
	}
	size_t TextureAsset::GetWidthSizeT() const
	{
		return Image_.GetMetadata().width;
	}
	size_t TextureAsset::GetHeightSizeT() const
	{
		return Image_.GetMetadata().height;
	}

	void TextureAsset::LoadFromCVMAT(const cv::Mat& MatToLoad)
	{
		CREATE_RESULT_HANDLE();
		CATCH_ERROR_DX(DirectX::LoadFromWICMemory(MatToLoad.ptr(), 1u, DirectX::WIC_FLAGS_NONE, nullptr, Image_));
	}
}

