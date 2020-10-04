//= Includes ===================================

#include "Flowpch.h"
#include "Material.h"

#include "Application.h"
#include "Rendering/Core/Bindables/Shaders/VertexShader.h"
#include "Rendering/Core/Bindables/Shaders/PixelShader.h"
#include "Rendering/Core/Bindables/InputLayout.h"
#include "Rendering/Core/Bindables/Texture.h"
#include "Rendering/Core/Bindables/Sampler.h"

#include "Rendering/Core/Renderable.h"
#include "GameFramework/Components/RenderableComponent.h"

#include "Assets/AssetSystem.h"
#include "Assets/Shaders/ShaderAsset.h"
#include "Assets/Textures/TextureAsset.h"

#include "Rendering/Core/Vertex/VertexLayout.h"

#include "Rendering/Core/RenderQueue/Step.h"

//= Class Definition - Material =============================

Material::Material()
	: m_Texture(nullptr)
	, m_PixelShader(nullptr)
	, m_VertexShader(nullptr)
{
}

void Material::BindMaterial(Step* RenderingStep, const VertexLayout& VertexLayout)
{
	RenderingStep->AddBindable(Texture::Resolve(m_Texture, 0));
	RenderingStep->AddBindable(Sampler::Resolve());

	auto vShader = VertexShader::Resolve(m_VertexShader->GetPath());
	auto vShaderByteCode = static_cast<VertexShader&>(*vShader).GetByteCode();
	RenderingStep->AddBindable(std::move(vShader));
	RenderingStep->AddBindable(PixelShader::Resolve(m_PixelShader->GetPath()));

	RenderingStep->AddBindable(InputLayout::Resolve(VertexLayout, vShaderByteCode));
}

void Material::SetTexture(const std::string& TextureName)
{
	auto NewTexture = AssetSystem::GetAsset<TextureAsset>(TextureName);

	CHECK_RETURN(!NewTexture, "Material::SetShader: Texture was nullptr");
	m_Texture = NewTexture;
}

void Material::SetPixelShader(const std::string& ShaderName)
{
	m_PixelShader = AssetSystem::GetAsset<ShaderAsset>(ShaderName);

}

void Material::SetVertexShader(const std::string& ShaderName)
{
	m_VertexShader = AssetSystem::GetAsset<ShaderAsset>(ShaderName);
}