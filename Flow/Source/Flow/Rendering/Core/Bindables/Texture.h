#pragma once
#include "Flow\Rendering\Core\Bindable.h"
#include "Flow/Assets/Textures/TextureAsset.h"

namespace Flow
{
	class Texture : public Bindable
	{
	public:

		Texture(TextureAsset* Asset);
		void Bind() override;

	protected:
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_TextureView;
	};
}