#pragma once
#include "Flow\Assets\AssetBase.h"
#include <string>
#include "DXTex\DirectXTex.h"

class TexColor
{
public:
	unsigned int _Data;

	constexpr TexColor()
		: _Data()
	{}

	constexpr TexColor(unsigned int DWord)
		: _Data(DWord)
	{}
};

class FLOW_API TextureAsset : public AssetBase
{

public:
	TextureAsset() {};
	~TextureAsset();

	virtual bool LoadAsset(const std::string& FilePath) override;

	unsigned int GetWidth() const { return (unsigned int)_Image.GetMetadata().width; }
	unsigned int GetHeight() const { return (unsigned int)_Image.GetMetadata().height; }

	unsigned int GetPitch() const { return (unsigned int)_Image.GetImage(0, 0, 0)->rowPitch; }

	uint8_t* GetBufferPtr() const { return _Image.GetPixels(); }

	const std::string& GetAssetPath() const { return _AssetPath; }

protected:

	DirectX::ScratchImage _Image;

	static constexpr DXGI_FORMAT _Format = DXGI_FORMAT::DXGI_FORMAT_B8G8R8A8_UNORM;
	std::string _AssetPath;
};