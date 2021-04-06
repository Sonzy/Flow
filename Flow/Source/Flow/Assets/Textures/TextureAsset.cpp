//= Includes ===========================

#include "pch.h"
#include <wincodec.h>
#include "TextureAsset.h"
#include "DXTex/DirectXTex.h"
#include "Utils/Timer.h"
#include "Utils/FileSystem.h"

#include "Rendering/Core/Bindables/Texture.h"

//= PreProcessor Macro Definitions ===================================

#define TIME_TEXTURE_CONVERSION 0

//= Class Definition - Texture Asset =====================

TextureAsset::~TextureAsset()
{
	delete m_Thumbnail;
}

bool TextureAsset::ImportAsset(const std::string& FilePath, const std::string& SavePath)
{
	HRESULT ResultHandle;

	//Load file using DirectXTex
	std::string Path = FilePath;
	std::wstring WidePath = std::wstring(Path.begin(), Path.end());
	CATCH_ERROR_DX(DirectX::LoadFromWICFile(WidePath.c_str(), DirectX::WIC_FLAGS_NONE, nullptr, m_Image));

	if (ResultHandle != 0)
	{
		// DX error already printed
		return false;
	}

	//Convert to our image format
	DXGI_FORMAT ImageFormat = m_Image.GetImage(0, 0, 0)->format;
	if (ImageFormat != m_Format)
	{
#if TIME_TEXTURE_CONVERSION
		Timer ConversionTimer;
#endif // TIME_TEXTURE_CONVERSION

		DirectX::ScratchImage ConvertedImage;
		CATCH_ERROR_DX(DirectX::Convert(*m_Image.GetImage(0, 0, 0), m_Format, DirectX::TEX_FILTER_DEFAULT, DirectX::TEX_THRESHOLD_DEFAULT, ConvertedImage));

		m_Image.InitializeFromImage(*ConvertedImage.GetImage(0, 0, 0));

#if TIME_TEXTURE_CONVERSION
		float Delta = ConversionTimer.Peek();
		FLOW_ENGINE_LOG("Converting {0} to {1}. Took {2} seconds", FilePath, m_Format, Delta);
#endif // TIME_TEXTURE_CONVERSION
	}

	//Update asset data
	m_AssetSize = sizeof(uint8_t) * m_Image.GetPixelsSize();
	m_AssetType = Asset::Type::Texture;
	m_AssetPath = FilePath;

	//save as png
	fs::path ConvertedPath = SavePath;
	ConvertedPath.replace_extension(".png");
	SaveAsset(ConvertedPath.string());

	//TODO: Save the thumbnail?
	// Create the thumbnail
	CreateThumbnail();

	return true;
}

bool TextureAsset::SaveAsset(const std::string& FilePath)
{	
	std::wstring conv = std::wstring(FilePath.begin(), FilePath.end());
	// We use basic image types for now.
	DirectX::SaveToWICFile(*m_Image.GetImage(0, 0, 0), DirectX::WIC_FLAGS_NONE, GUID_ContainerFormatPng, conv.c_str());
	return false;
}

bool TextureAsset::LoadAsset(const std::string& FilePath)
{
	HRESULT ResultHandle;

	//Load file using DirectXTex
	std::string Path = FilePath;
	std::wstring WidePath = std::wstring(Path.begin(), Path.end());
	CATCH_ERROR_DX(DirectX::LoadFromWICFile(WidePath.c_str(), DirectX::WIC_FLAGS_NONE, nullptr, m_Image));

	if (ResultHandle != 0)
	{
		// DX error already printed
		return false;
	}

	//Convert to our image format
	DXGI_FORMAT ImageFormat = m_Image.GetImage(0, 0, 0)->format;
	if (ImageFormat != m_Format)
	{
#if TIME_TEXTURE_CONVERSION
		Timer ConversionTimer;
#endif // TIME_TEXTURE_CONVERSION

		DirectX::ScratchImage ConvertedImage;
		CATCH_ERROR_DX(DirectX::Convert(*m_Image.GetImage(0, 0, 0), m_Format, DirectX::TEX_FILTER_DEFAULT, DirectX::TEX_THRESHOLD_DEFAULT, ConvertedImage));

		m_Image.InitializeFromImage(*ConvertedImage.GetImage(0, 0, 0));

#if TIME_TEXTURE_CONVERSION
		float Delta = ConversionTimer.Peek();
		FLOW_ENGINE_LOG("Converting {0} to {1}. Took {2} seconds", FilePath, m_Format, Delta);
#endif // TIME_TEXTURE_CONVERSION
	}

	//Update asset data
	m_AssetSize = sizeof(uint8_t) * m_Image.GetPixelsSize();
	m_AssetType = Asset::Type::Texture;
	m_AssetPath = FilePath;

	//Create thumbnail
	CreateThumbnail();

	return true;
}

void TextureAsset::CreateThumbnail()
{
	HRESULT ResultHandle;

	CATCH_ERROR_DX(DirectX::Resize(*m_Image.GetImage(0, 0, 0), 128, 128, DirectX::TEX_FILTER_DEFAULT, m_ThumbnailImage));
	m_Thumbnail = new Texture(m_ThumbnailImage, 0, m_AssetName + "_Thumnail");
}
