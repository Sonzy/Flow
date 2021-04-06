#include "pch.h"
#include "Mat_Texture2D.h"

#include "Rendering/Core/Bindables/Shaders/VertexShader.h"
#include "Rendering/Core/Bindables/Shaders/PixelShader.h"
#include "Rendering/Core/Bindables/Sampler.h"
#include "Rendering/Core/Bindables/Texture.h"
#include "Rendering/Core/Bindables/InputLayout.h"
#include "Rendering/Core/Bindables/ConstantBuffers/ShaderConstantBuffers.h"

#include "Assets/Shaders/ShaderAsset.h"
#include "Assets/Textures/TextureAsset.h"

#include "Rendering/Core/RenderQueue/Step.h"

Mat_Texture2D::Mat_Texture2D()
{
	SetTexture("QuadTextureTest");
	SetPixelShader("Texture2D_PS");
	SetVertexShader("Texture2D_VS");
}

void Mat_Texture2D::BindMaterial(Step* RenderingStep, const VertexLayout& VertexLayout) const
{
	RenderingStep->AddBindable(Texture::Resolve(m_Texture, 0));
	RenderingStep->AddBindable(Sampler::Resolve());
	auto vShader = VertexShader::Resolve(m_VertexShader->GetPath());
	auto vShaderByteCode = static_cast<VertexShader&>(*vShader).GetByteCode();
	RenderingStep->AddBindable(std::move(vShader));
	RenderingStep->AddBindable(PixelShader::Resolve(m_PixelShader->GetPath()));
	RenderingStep->AddBindable(InputLayout::Resolve(VertexLayout, vShaderByteCode));
}

std::string Mat_Texture2D::GenerateTag() const
{
	return std::string("Texture2D_" + m_Texture->GetAssetPath());
}
