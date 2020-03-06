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

		static std::shared_ptr<Bindable> Resolve(TextureAsset* Asset, UINT slot);
		static std::string GenerateUID(TextureAsset* Asset, UINT slot);
		std::string GetUID() const override;

	protected:
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_TextureView;
		UINT m_Slot;

		std::string m_AssetName;//Used to generate the UID
	};
}