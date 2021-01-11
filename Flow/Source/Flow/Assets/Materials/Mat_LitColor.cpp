#include "Flowpch.h"
#include "Mat_LitColor.h"

//= Bindables =
#include "Rendering/Core/Bindables/Shaders/VertexShader.h"
#include "Rendering/Core/Bindables/Shaders/PixelShader.h"
#include "Rendering/Core/Bindables/Sampler.h"
#include "Rendering/Core/Bindables/Texture.h"
#include "Rendering/Core/Bindables/InputLayout.h"
#include "Rendering/Core/Bindables/ConstantBuffers/ShaderConstantBuffers.h"

//= Assets =
#include "Assets\AssetSystem.h"
#include "Assets\Shaders\ShaderAsset.h"
#include "Assets\Textures\TextureAsset.h"

//= Rendering =
#include "Rendering/Core/RenderQueue/Step.h"

Mat_LitColor::Mat_LitColor()
{
	m_color = { 1.0f, 1.0f, 1.0f, 1.0f };
	m_lightProperties =
	{
		{1.0f, 1.0f, 1.0f},
		0.1f,
		20.0f,
		{0.0f, 0.0f, 0.0f}
	};

	SetVertexShader("LitColor_VS");
	SetPixelShader("LitColor_PS");
}

void Mat_LitColor::SetColor(Vector3 NewColour)
{
	m_color = { NewColour.x, NewColour.y, NewColour.z, 1.0f };
}

void Mat_LitColor::BindMaterial(Step* RenderingStep, const VertexLayout& VertexLayout)
{
	auto vShader = VertexShader::Resolve(m_VertexShader->GetPath());
	auto vShaderByteCode = static_cast<VertexShader&>(*vShader).GetByteCode();
	RenderingStep->AddBindable(std::move(vShader));
	RenderingStep->AddBindable(PixelShader::Resolve(m_PixelShader->GetPath()));
	RenderingStep->AddBindable(InputLayout::Resolve(VertexLayout, vShaderByteCode));

	RenderingStep->AddBindable(PixelConstantBuffer<MaterialCommon::Buffer::ColorFloat4>::Resolve(m_color, MaterialCommon::Register::ColorFloat4, GenerateTag()));
	RenderingStep->AddBindable(PixelConstantBuffer<MaterialCommon::Buffer::ObjectLightProperties>::Resolve(m_lightProperties, MaterialCommon::Register::ObjectLightProperties));
}

std::string Mat_LitColor::GenerateTag()
{
	return std::string("LitColor") +
		std::to_string(m_color.r) +
		std::to_string(m_color.g) +
		std::to_string(m_color.b) +
		std::to_string(m_color.a);
}
