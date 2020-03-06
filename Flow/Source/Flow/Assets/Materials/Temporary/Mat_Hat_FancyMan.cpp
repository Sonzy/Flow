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

	void Flow::Mat_Hat_FancyMan::BindMaterial(StaticMesh* Parent, const VertexLayout& VertexLayout)
	{
		CHECK_RETURN(!Parent, "Mat_FlatColour::BindMaterial: Parent was nullptr");

		Parent->AddBind(Texture::Resolve(m_Texture, 0));
		Parent->AddBind(Sampler::Resolve());
		auto vShader = VertexShader::Resolve(m_VertexShader->GetPath());
		auto vShaderByteCode = static_cast<VertexShader&>(*vShader).GetByteCode();
		Parent->AddBind(std::move(vShader));
		Parent->AddBind(PixelShader::Resolve(m_PixelShader->GetPath()));
		Parent->AddBind(InputLayout::Resolve(VertexLayout, vShaderByteCode));
	}
}