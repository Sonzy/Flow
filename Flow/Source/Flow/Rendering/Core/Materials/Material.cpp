#include "Flowpch.h"
#include "Material.h"

#include "Flow\Application.h"
#include "Flow/Rendering/Core/Bindables/Shaders/VertexShader.h"
#include "Flow/Rendering/Core/Bindables/Shaders/PixelShader.h"


#include "Flow\Assets\AssetSystem.h"
#include "Flow\Assets\Shaders\ShaderAsset.h"
#include "Flow\Assets\Textures\TextureAsset.h"

#include "Flow\Rendering\Core\Renderable.h"

namespace Flow
{
	Material::Material()
	{
	}

	void Material::BindMaterial(StaticMesh* Parent, const VertexLayout& VertexLayout)
	{
		Parent->AddBind(Texture::Resolve(m_Texture, 0));
		Parent->AddBind(Sampler::Resolve());

		auto vShader = VertexShader::Resolve(m_VertexShader->GetPath());
		auto vShaderByteCode = static_cast<VertexShader&>(*vShader).GetByteCode();
		Parent->AddBind(std::move(vShader));
		Parent->AddBind(PixelShader::Resolve(m_PixelShader->GetPath()));

		Parent->AddBind(InputLayout::Resolve(VertexLayout, vShaderByteCode));
	}

	void Material::SetTexture(const std::string& TextureName)
	{
		auto Temp = AssetSystem::GetAsset<TextureAsset>(TextureName);

		CHECK_RETURN(!Temp, "Material::SetShader: Texture was nullptr");
		m_Texture = Temp;
	}

	void Material::SetPixelShader(const std::string& ShaderName)
	{
		m_PixelShader = AssetSystem::GetAsset<ShaderAsset>(ShaderName);

	}

	void Material::SetVertexShader(const std::string& ShaderName)
	{
		m_VertexShader = AssetSystem::GetAsset<ShaderAsset>(ShaderName);
	}
}

