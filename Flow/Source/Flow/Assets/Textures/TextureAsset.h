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

		unsigned int GetWidth() { return m_TextureWidth; }
		unsigned int GetHeight() { return m_TextureHeight; }

		TexColor* GetBufferPtr() { return m_TextureBuffer.get(); }
		

	protected:

		std::unique_ptr< TexColor[]> m_TextureBuffer;

		unsigned int m_TextureWidth = 0;
		unsigned int m_TextureHeight = 0;
	};
}