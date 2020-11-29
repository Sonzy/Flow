#include "Flowpch.h"
#include "ColorMaterial2D.h"

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

ColorMaterial2D::ColorMaterial2D()
{
	m_Color = { DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) };

	SetVertexShader("SolidColor2DVS");
	SetPixelShader("SolidColor2DPS");
}

void ColorMaterial2D::SetColour(Vector3 NewColour)
{
	m_Color = { DirectX::XMFLOAT4(NewColour.x, NewColour.y, NewColour.z, 1.0f) };
}

void ColorMaterial2D::BindMaterial(Step* RenderingStep, const VertexLayout& VertexLayout)
{
	auto vShader = VertexShader::Resolve(m_VertexShader->GetPath());
	auto vShaderByteCode = static_cast<VertexShader&>(*vShader).GetByteCode();
	RenderingStep->AddBindable(std::move(vShader));
	RenderingStep->AddBindable(PixelShader::Resolve(m_PixelShader->GetPath()));
	RenderingStep->AddBindable(InputLayout::Resolve(VertexLayout, vShaderByteCode));

	RenderingStep->AddBindable(PixelConstantBuffer<ColorBuffer>::Resolve(m_Color, 2u, GenerateTag()));
}

std::string ColorMaterial2D::GenerateTag()
{
	return std::string("FlatColour") +
		std::to_string(m_Color.Colour.x) +
		std::to_string(m_Color.Colour.y) +
		std::to_string(m_Color.Colour.z) +
		std::to_string(m_Color.Colour.w);
}

