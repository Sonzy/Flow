#pragma once

// Includes ////////////////////////////////////////////////////////////

#include "Framework/Types/ComPtr.h"
#include "DXTex/DirectXTex.h"
#include "Rendering/Core/Bindables/Bindable.h"

// Type Definitions ////////////////////////////////////////////////////////////

struct ID3D11ShaderResourceView;
class TextureAsset;

// Class Definitions ////////////////////////////////////////////////////////////

namespace Bindables
{
	class Texture : public Bindables::Bindable
	{
	public:

		// Public Static Functions ////////////////////////////////////////////////////////////

		static Texture*							Resolve(TextureAsset* asset, UINT slot);
		static HashString						GenerateID(TextureAsset* asset, UINT slot);

	public:

		// Public Functions ////////////////////////////////////////////////////////////

												Texture(TextureAsset* asset, UINT slot);
												Texture(const DirectX::ScratchImage& image, UINT slot, const std::string& assetName);

		virtual void							Bind() override;
		ID3D11ShaderResourceView*				GetTextureView() const;

		// Bindable Interface =

		virtual HashString						GetID() override;

	protected:

		// Protected Variables ////////////////////////////////////////////////////////////

		ComPtr<ID3D11Texture2D>					m_texture;
		ComPtr<ID3D11ShaderResourceView>		m_textureView;
		uint32									m_slot;
		TextureAsset*							m_asset;
	};
}