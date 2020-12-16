#include "Flowpch.h"
#include "Mat_FlatColor_2D.h"

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

Mat_FlatColor_2D::Mat_FlatColor_2D()
{
	m_Color = { DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) };

	SetVertexShader("SolidColor2D_VS");
	SetPixelShader("SolidColor2D_PS");
}

void Mat_FlatColor_2D::SetColour(Vector3 NewColor)
{
}

void Mat_FlatColor_2D::BindMaterial(Step* RenderingStep, const VertexLayout& VertexLayout)
{
	auto vShader = VertexShader::Resolve(m_VertexShader->GetPath());
	auto vShaderByteCode = static_cast<VertexShader&>(*vShader).GetByteCode();
	RenderingStep->AddBindable(std::move(vShader));
	RenderingStep->AddBindable(PixelShader::Resolve(m_PixelShader->GetPath()));
	RenderingStep->AddBindable(InputLayout::Resolve(VertexLayout, vShaderByteCode));

	RenderingStep->AddBindable(PixelConstantBuffer<ColorBuffer>::Resolve(m_Color, 2u, GenerateTag()));
}

std::string Mat_FlatColor_2D::GenerateTag()
{
	return std::string("FlatColour2D") +
		std::to_string(m_Color.Color.x) +
		std::to_string(m_Color.Color.y) +
		std::to_string(m_Color.Color.z) +
		std::to_string(m_Color.Color.w);
}
