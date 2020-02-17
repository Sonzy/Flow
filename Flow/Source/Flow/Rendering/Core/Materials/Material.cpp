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
	Material::Material(StaticMesh* Parent)
	{
		CHECK_RETURN(!Parent, "Material::Material: Parent was nullptr");

		m_Parent = Parent;
	}


	void Material::BindMaterial(const VertexBuffer& VB)
	{
		m_Parent->AddStaticBindable(std::make_unique<Texture>(m_Texture));
		m_Parent->AddStaticBindable(std::make_unique<Sampler>());

		auto vShader = std::make_unique<VertexShader>(m_VertexShader->GetPath());
		auto vShaderByteCode = vShader->GetByteCode();
		m_Parent->AddStaticBindable(std::move(vShader));
		m_Parent->AddStaticBindable(std::make_unique<PixelShader>(m_PixelShader->GetPath()));

		m_Parent->AddStaticBindable(std::make_unique<InputLayout>(VB.GetLayout().GetD3DLayout(), vShaderByteCode));
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

