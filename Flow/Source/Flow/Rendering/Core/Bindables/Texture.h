#pragma once
#include "Flow\Rendering\Core\Bindable.h"
#include "Flow/Assets/Textures/TextureAsset.h"

namespace Flow
{
	class Texture : public Bindable
	{
	public:

		Texture(TextureAsset* Asset, UINT slot);
		void Bind() override;

		//= Bindable Interface =

		void Update(TextureAsset* Tex);

		static std::shared_ptr<Bindable> Resolve(TextureAsset* Asset, UINT slot);
		static std::string GenerateUID(TextureAsset* Asset, UINT slot);
		std::string GetUID() const override;

		ID3D11ShaderResourceView* GetTextureUnsafe() { return TextureView_.Get(); }
	protected:
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> TextureView_;
		Microsoft::WRL::ComPtr<ID3D11Texture2D> pTexture;
		UINT Slot_;

		std::string AssetName_;//Used to generate the UID
	};
}