#pragma once
#include "Flow\Assets\AssetBase.h"
#include <string>

namespace Flow
{
	//TODO: Come up with a better colour/texture class
	class TexColor
	{
	public:
		unsigned int m_DWord;

		constexpr TexColor()
			: m_DWord()
		{}

		constexpr TexColor(unsigned int DWord)
			: m_DWord(DWord)
		{}
	};

	class TextureAsset : public AssetBase
	{

	public:
		TextureAsset() {};

		virtual bool LoadAsset(const std::string& FilePath) override;

		unsigned int GetWidth() const { return m_TextureWidth; }
		unsigned int GetHeight() const { return m_TextureHeight; }

		TexColor* GetBufferPtr() const { return m_TextureBuffer.get(); }

		const std::string& GetAssetPath() const { return m_AssetPath; }		

	protected:

		std::unique_ptr< TexColor[]> m_TextureBuffer;

		unsigned int m_TextureWidth = 0;
		unsigned int m_TextureHeight = 0;

		std::string m_AssetPath;
	};
}