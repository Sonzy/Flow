#pragma once
#include "Flow\Assets\AssetBase.h"
#include <string>
#include "DXTex\DirectXTex.h"

namespace Flow
{
	class TexColor
	{
	public:
		unsigned int Data_;

		constexpr TexColor()
			: Data_()
		{}

		constexpr TexColor(unsigned int DWord)
			: Data_(DWord)
		{}

		constexpr TexColor(unsigned char r, unsigned char g, unsigned char b) noexcept
			:
			Data_((255u << 24u) | (r << 16u) | (g << 8u) | b)
		{}
	};

	class FLOW_API TextureAsset : public AssetBase
	{

	public:
		TextureAsset() {};
		~TextureAsset();

		virtual bool LoadAsset(const std::string& FilePath) override;

		unsigned int GetWidth() const { return (unsigned int)Image_.GetMetadata().width; }
		unsigned int GetHeight() const { return (unsigned int)Image_.GetMetadata().height; }

		unsigned int GetPitch() const { return (unsigned int)Image_.GetImage(0, 0, 0)->rowPitch; }

		uint8_t* GetBufferPtr() const { return Image_.GetPixels(); }

		const std::string& GetAssetPath() const { return m_AssetPath; }		


		void AlterPixel(unsigned int x, unsigned int y, TexColor Colour);

		//DONT USE, TEMP Hack for the project
		void OverwriteAssetPath(const std::string& NewPath);

	protected:

		DirectX::ScratchImage Image_;

		static constexpr DXGI_FORMAT Format_ = DXGI_FORMAT::DXGI_FORMAT_B8G8R8A8_UNORM;//DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
		std::string m_AssetPath;
	};
}