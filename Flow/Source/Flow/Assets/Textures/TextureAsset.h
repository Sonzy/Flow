#pragma once
#include <string>
#include "Flow\Assets\Asset.h"
#include "DXTex\DirectXTex.h"



class FLOW_API TextureAsset : public Asset
{

public:

	//= Public Functions =======================================

									TextureAsset() {};
									~TextureAsset();

	virtual bool					ImportAsset(const std::string& FilePath, const std::string& SavePath) override;
	virtual bool					SaveAsset(const std::string& FilePath) override;
	virtual bool					LoadAsset(const std::string& FilePath) override;

	unsigned int					GetWidth() const { return static_cast<unsigned int>(m_Image.GetMetadata().width); }
	unsigned int					GetHeight() const { return static_cast<unsigned int>(m_Image.GetMetadata().height); }
	unsigned int					GetPitch() const { return static_cast<unsigned int>(m_Image.GetImage(0, 0, 0)->rowPitch); }

	uint8_t*						GetBufferPtr() const { return m_Image.GetPixels(); }
	const std::string&				GetAssetPath() const { return m_AssetPath; }

	const Texture*					GetThumbnail() const { return m_Thumbnail; }

protected:
	//= Protected Functions ===================================

	virtual void					CreateThumbnail() override;

protected:

	//= Protected Variables ===================================

	DirectX::ScratchImage			m_Image;
	static constexpr DXGI_FORMAT	m_Format = DXGI_FORMAT::DXGI_FORMAT_B8G8R8A8_UNORM; //Windows default format
	std::string						m_AssetPath;
	DirectX::ScratchImage			m_ThumbnailImage;
	Texture*						m_Thumbnail;
};