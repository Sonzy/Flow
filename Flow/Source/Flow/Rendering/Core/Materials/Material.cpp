#include "Flowpch.h"
#include "Material.h"

#include "Flow\Application.h"
#include "Flow/Rendering/Core/Bindables/Shaders/VertexShader.h"
#include "Flow/Rendering/Core/Bindables/Shaders/PixelShader.h"
#include "Flow\Rendering\Core\Bindables\InputLayout.h"
#include "Flow\Rendering\Core\Bindables\Texture.h"
#include "Flow\Rendering\Core\Bindables\Sampler.h"

#include "Flow/Rendering/Core/Renderable.h"
#include "Flow\GameFramework\Components\RenderableComponent.h"

#include "Flow\Assets\AssetSystem.h"
#include "Flow\Assets\Shaders\ShaderAsset.h"
#include "Flow\Assets\Textures\TextureAsset.h"

#include "Flow\Rendering\Core\Vertex\VertexLayout.h"

Material::Material()
{
}

void Material::BindMaterial(Renderable* Parent, const VertexLayout& VertexLayout)
{
	Parent->AddBind(Texture::Resolve(_Texture, 0));
	Parent->AddBind(Sampler::Resolve());

	auto vShader = VertexShader::Resolve(_VertexShader->GetPath());
	auto vShaderByteCode = static_cast<VertexShader&>(*vShader).GetByteCode();
	Parent->AddBind(std::move(vShader));
	Parent->AddBind(PixelShader::Resolve(_PixelShader->GetPath()));

	Parent->AddBind(InputLayout::Resolve(VertexLayout, vShaderByteCode));
}

void Material::BindMaterial(RenderableComponent* Parent, const VertexLayout& VertexLayout)
{
	Parent->AddBind(Texture::Resolve(_Texture, 0));
	Parent->AddBind(Sampler::Resolve());

	auto VShader = VertexShader::Resolve(_VertexShader->GetPath());
	auto VShaderByteCode = static_cast<VertexShader&>(*VShader).GetByteCode();
	Parent->AddBind(std::move(VShader));
	Parent->AddBind(PixelShader::Resolve(_PixelShader->GetPath()));

	Parent->AddBind(InputLayout::Resolve(VertexLayout, VShaderByteCode));
}

void Material::SetTexture(const std::string& TextureName)
{
	auto NewTexture = AssetSystem::GetAsset<TextureAsset>(TextureName);

	CHECK_RETURN(!NewTexture, "Material::SetShader: Texture was nullptr");
	_Texture = NewTexture;
}

void Material::SetPixelShader(const std::string& ShaderName)
{
	_PixelShader = AssetSystem::GetAsset<ShaderAsset>(ShaderName);

}

void Material::SetVertexShader(const std::string& ShaderName)
{
	_VertexShader = AssetSystem::GetAsset<ShaderAsset>(ShaderName);
}