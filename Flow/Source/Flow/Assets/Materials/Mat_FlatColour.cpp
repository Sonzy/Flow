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
	m_Color = { DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) };

	SetVertexShader("SolidColourVS");
	SetPixelShader("SolidColourPS");
}

void Mat_FlatColour::SetColour(Vector3 NewColour)
{
	m_Color = { DirectX::XMFLOAT4(NewColour.x, NewColour.y, NewColour.z, 1.0f) };
}

void Mat_FlatColour::BindMaterial(Step* RenderingStep, const VertexLayout& VertexLayout)
{
	auto vShader = VertexShader::Resolve(_VertexShader->GetPath());
	auto vShaderByteCode = static_cast<VertexShader&>(*vShader).GetByteCode();
	RenderingStep->AddBindable(std::move(vShader));
	RenderingStep->AddBindable(PixelShader::Resolve(_PixelShader->GetPath()));
	RenderingStep->AddBindable(InputLayout::Resolve(VertexLayout, vShaderByteCode));

	RenderingStep->AddBindable(PixelConstantBuffer<ColorBuffer>::Resolve(m_Color, 2u, GenerateTag()));
}

std::string Mat_FlatColour::GenerateTag()
{
	return std::string("FlatColour") +
		std::to_string(m_Color.Colour.x) +
		std::to_string(m_Color.Colour.y) +
		std::to_string(m_Color.Colour.z) +
		std::to_string(m_Color.Colour.w);
}

