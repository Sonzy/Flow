#pragma once

//= Includes =============================================

#include "Flow\Rendering\Core\Bindable.h"
#include "Flow/Assets/Textures/TextureAsset.h"

//= Class Definitions ====================================

class Texture : public Bindable
{
public:

	//= Public Static Functions ==================================================

	static std::shared_ptr<Bindable>		Resolve(TextureAsset* Asset, UINT slot);
	static std::string						GenerateUID(TextureAsset* Asset, UINT slot);

public:

	//= Public Functions =========================================================

											Texture(TextureAsset* Asset, UINT slot);
	void									Bind() override;
	ID3D11ShaderResourceView*				GetTextureView() const { return m_TextureView.Get(); } //TODO: being lazy, can go out of scope whilst in use

	//= Bindable Interface =

	std::string								GetUID() const override;

protected:

	//= Protected Variables =======================================================

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>		m_TextureView;
	UINT													m_Slot;
	std::string												m_AssetName;
};