#pragma once
#include "Flow\Rendering\Core\Bindable.h"
#include "Flow/Assets/Textures/TextureAsset.h"

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
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> _TextureView;
	UINT _Slot;

	std::string _AssetName;//Used to generate the UID
};