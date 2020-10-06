//= Includes ===========================

#include "Flowpch.h"
#include <wincodec.h>
#include "TextureAsset.h"
#include "DXTex/DirectXTex.h"
#include "Utils/Timer.h"

//= Class Definition - Texture Asset =====================

TextureAsset::~TextureAsset()
{
}

bool TextureAsset::ImportAsset(const std::string& FilePath)
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
#ifdef TIME_TEXTURE_CONVERSION
		Timer ConversionTimer;
#endif // TIME_TEXTURE_CONVERSION

		DirectX::ScratchImage ConvertedImage;
		CATCH_ERROR_DX(DirectX::Convert(*m_Image.GetImage(0, 0, 0), m_Format, DirectX::TEX_FILTER_DEFAULT, DirectX::TEX_THRESHOLD_DEFAULT, ConvertedImage));

		m_Image.InitializeFromImage(*ConvertedImage.GetImage(0, 0, 0));

#ifdef TIME_TEXTURE_CONVERSION
		float Delta = ConversionTimer.Peek();
		FLOW_ENGINE_LOG("Converting {0} to {1}. Took {2} seconds", FilePath, m_Format, Delta);
#else
		FLOW_ENGINE_LOG("Converting {0} to {1}", FilePath, m_Format);
#endif // TIME_TEXTURE_CONVERSION
	}

	//Update asset data
	m_AssetSize = sizeof(uint8_t) * m_Image.GetPixelsSize();
	m_AssetType = Asset::Type::Texture;
	m_AssetPath = FilePath;

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
	// We use basic image types for now.
	return ImportAsset(FilePath);
}
