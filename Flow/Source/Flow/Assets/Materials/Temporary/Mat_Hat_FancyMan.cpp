#include "Flowpch.h"
#include "Mat_Hat_FancyMan.h"

namespace Flow
{
	Flow::Mat_Hat_FancyMan::Mat_Hat_FancyMan()
	{
		SetTexture("CharacterTexture");
		SetPixelShader("TexturedPS");
		SetVertexShader("TexturedVS");
	}

	void Flow::Mat_Hat_FancyMan::BindMaterial(StaticMesh* Parent, const std::vector<D3D11_INPUT_ELEMENT_DESC>& VertexLayout)
	{
		CHECK_RETURN(!Parent, "Mat_FlatColour::BindMaterial: Parent was nullptr");

		Parent->ReplaceOrAddStaticBindable<Texture>(std::make_unique<Texture>(m_Texture));
		Parent->ReplaceOrAddStaticBindable<Sampler>(std::make_unique<Sampler>());

		auto vShader = std::make_unique<VertexShader>(m_VertexShader->GetPath());
		auto vShaderByteCode = vShader->GetByteCode();
		Parent->ReplaceOrAddStaticBindable<VertexShader>(std::move(vShader));
		Parent->ReplaceOrAddStaticBindable<PixelShader>(std::make_unique<PixelShader>(m_PixelShader->GetPath()));
		Parent->ReplaceOrAddStaticBindable<InputLayout>(std::make_unique<InputLayout>(VertexLayout, vShaderByteCode));

		if (Parent->StaticInitialised())
			Parent->RefreshStaticBinds();
	}
}