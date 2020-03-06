#include "Flowpch.h"
#include "TextureAsset.h"

#include "Flow\Application.h"

#include <gdiplus.h>
#pragma comment( lib,"gdiplus.lib" )

namespace Flow
{
	TextureAsset::~TextureAsset()
	{
	}

	bool TextureAsset::LoadAsset(const std::string& FilePath)
	{
		m_TextureBuffer.release();

		//Convert to Wstring and load as a bitmap
		std::string Path = Application::GetApplication().GetLocalFilePath() + FilePath;
		std::wstring WidePath = std::wstring(Path.begin(), Path.end());

		Gdiplus::Bitmap BitMapTexture(WidePath.c_str());
		Gdiplus::Status Status = BitMapTexture.GetLastStatus();

		CHECK_RETURN_FALSE(Status != Gdiplus::Status::Ok, ("TextureAsset::LoadTexture: Failed to load asset {0} {1}", Status, Path));

		m_TextureWidth = BitMapTexture.GetWidth();
		m_TextureHeight = BitMapTexture.GetHeight();
		m_TextureBuffer = std::make_unique<TexColor[]>(m_TextureWidth * m_TextureHeight);

		//Load image into buffer
		for (unsigned int y = 0; y < m_TextureHeight; y++)
		{
			for (unsigned int x = 0; x < m_TextureWidth; x++)
			{
				Gdiplus::Color c;
				BitMapTexture.GetPixel(x, y, &c);
				m_TextureBuffer[y * m_TextureWidth + x] = c.GetValue();
			}
		}

		m_AssetSize = sizeof(TexColor) * (m_TextureHeight * m_TextureWidth);
		m_AssetType = EAssetType::Texture;
		FLOW_ENGINE_LOG("MeshAsset::LoadAsset: Successfully loaded texture {0} (Size: {1})", FilePath, m_AssetSize);

		m_AssetPath = FilePath;

		return true;
	}
}

