#include "Flowpch.h"
#include "TextureAsset.h"

#include "Flow\Application.h"

#include <gdiplus.h>
#pragma comment( lib,"gdiplus.lib" )

namespace Flow
{
	bool TextureAsset::LoadAsset(const std::string& FilePath)
	{
		m_TextureBuffer.release();

		//Convert to Wstring and load as a bitmap
		std::wstring WidePath = Application::GetApplication().GetLocalFilePathWide();
		WidePath.append(std::wstring(FilePath.begin(), FilePath.end()));
		Gdiplus::Bitmap BitMapTexture(WidePath.c_str());

		Gdiplus::Status Status = BitMapTexture.GetLastStatus();
		CHECK_RETURN_FALSE(Status != Gdiplus::Status::Ok,
			(L"TextureAsset::LoadTexture: Failed to load asset {0} {1}", Status, std::string(WidePath.begin(), WidePath.end())));

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

		FLOW_ENGINE_LOG("TextureAsset::LoadAsset: Successfully loaded texture {0}", std::string(WidePath.begin(), WidePath.end()));

		return true;
	}
}

