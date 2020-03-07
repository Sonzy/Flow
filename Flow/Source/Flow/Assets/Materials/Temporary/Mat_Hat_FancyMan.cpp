#include "Flowpch.h"
#include "Mat_Hat_FancyMan.h"
#include "Flow/Rendering/Core/Bindables/Shaders/VertexShader.h"
#include "Flow/Rendering/Core/Bindables/Shaders/PixelShader.h"

#include "Flow/Rendering/Core/Mesh/StaticMesh.h"
#include "Flow\GameFramework\Components\RenderableComponent.h"


#include "Flow\Assets\AssetSystem.h"
#include "Flow\Assets\Shaders\ShaderAsset.h"
#include "Flow\Assets\Textures\TextureAsset.h"

#include "Flow\Rendering\Core\Vertex\VertexLayout.h"

namespace Flow
{
	Flow::Mat_Hat_FancyMan::Mat_Hat_FancyMan()
	{
		SetTexture("CharacterTexture");
		SetPixelShader("TexturedPhongPS");
		SetVertexShader("TexturedPhongVS");

		buff =
		{
			{1.0f, 1.0f, 1.0f},
			0.1f,
			20.0f,
			{0.0f, 0.0f, 0.0f}
		};
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


		Parent->AddBind(PixelConstantBuffer<ObjectLightBuffer>::Resolve(buff, 1u));
	}

	void Mat_Hat_FancyMan::BindMaterial(RenderableComponent* Parent, const VertexLayout& VertexLayout)
	{
		CHECK_RETURN(!Parent, "Mat_FlatColour::BindMaterial: Parent was nullptr");

		Parent->AddBind(Texture::Resolve(m_Texture, 0));
		Parent->AddBind(Sampler::Resolve());
		auto vShader = VertexShader::Resolve(m_VertexShader->GetPath());
		auto vShaderByteCode = static_cast<VertexShader&>(*vShader).GetByteCode();
		Parent->AddBind(std::move(vShader));
		Parent->AddBind(PixelShader::Resolve(m_PixelShader->GetPath()));
		Parent->AddBind(InputLayout::Resolve(VertexLayout, vShaderByteCode));


		Parent->AddBind(PixelConstantBuffer<ObjectLightBuffer>::Resolve(buff, 1u));
	}
}