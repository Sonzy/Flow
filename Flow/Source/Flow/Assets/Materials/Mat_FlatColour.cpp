#include "Flowpch.h"
#include "Mat_FlatColour.h"

#include "Rendering/Core/Bindables/Shaders/VertexShader.h"
#include "Rendering/Core/Bindables/Shaders/PixelShader.h"
#include "Rendering/Core/Bindables/Sampler.h"
#include "Rendering/Core/Bindables/Texture.h"
#include "Rendering/Core/Bindables/InputLayout.h"
#include "Rendering/Core/Bindables/ConstantBuffers/ShaderConstantBuffers.h"

#include "Assets\AssetSystem.h"
#include "Assets\Shaders\ShaderAsset.h"
#include "Assets\Textures\TextureAsset.h"

#include "Rendering\Core\Vertex\VertexLayout.h"

#include "Rendering/Core/RenderQueue/Step.h"

Mat_FlatColour::Mat_FlatColour()
{
	m_color = { 1.0f, 1.0f, 1.0f, 1.0f };

	SetVertexShader("SolidColor_VS");
	SetPixelShader("SolidColor_PS");
}

void Mat_FlatColour::SetColour(Vector3 NewColour)
{
	m_color = { NewColour.x, NewColour.y, NewColour.z, 1.0f };
}

void Mat_FlatColour::BindMaterial(Step* RenderingStep, const VertexLayout& VertexLayout) const
{
	auto vShader = VertexShader::Resolve(m_VertexShader->GetPath());
	auto vShaderByteCode = static_cast<VertexShader&>(*vShader).GetByteCode();
	RenderingStep->AddBindable(std::move(vShader));
	RenderingStep->AddBindable(PixelShader::Resolve(m_PixelShader->GetPath()));
	RenderingStep->AddBindable(InputLayout::Resolve(VertexLayout, vShaderByteCode));

	RenderingStep->AddBindable(PixelConstantBuffer<MaterialCommon::Buffer::ColorFloat4>::Resolve(m_color, 0, GenerateTag()));
}

std::string Mat_FlatColour::GenerateTag() const
{
	return std::string("FlatColor") +
		std::to_string(m_color.r) +
		std::to_string(m_color.g) +
		std::to_string(m_color.b) +
		std::to_string(m_color.a);
}

