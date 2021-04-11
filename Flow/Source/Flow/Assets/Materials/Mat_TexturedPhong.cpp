#include "pch.h"
#include "Mat_TexturedPhong.h"

#include "Rendering/Core/Bindables/Shaders/VertexShader.h"
#include "Rendering/Core/Bindables/Shaders/PixelShader.h"
#include "Rendering/Core/Bindables/Sampler.h"
#include "Rendering/Core/Bindables/Texture.h"
#include "Rendering/Core/Bindables/InputLayout.h"
#include "Rendering/Core/Bindables/ConstantBuffers/ShaderConstantBuffers.h"

#include "Assets/Shaders/ShaderAsset.h"

#include "Rendering/Core/RenderQueue/Step.h"

Mat_TexturedPhong::Mat_TexturedPhong()
{
	SetTexture("Wabble_Props");
	SetPixelShader("TexturedPhong_PS");
	SetVertexShader("TexturedPhong_VS");

	m_lightProperties =
	{
		{1.0f, 1.0f, 1.0f},
		0.1f,
		20.0f,
		{0.0f, 0.0f, 0.0f}
	};
}

void Mat_TexturedPhong::BindMaterial(Step* RenderingStep, const VertexLayout& VertexLayout) const
{
	RenderingStep->AddBindable(Bindables::Texture::Resolve(m_Texture, 0));
	RenderingStep->AddBindable(Sampler::Resolve());
	auto vShader = Bindables::VertexShader::Resolve(m_VertexShader->GetPath());
	auto vShaderByteCode = static_cast<Bindables::VertexShader&>(*vShader).GetByteCode();
	RenderingStep->AddBindable(std::move(vShader));
	RenderingStep->AddBindable(Bindables::PixelShader::Resolve(m_PixelShader->GetPath()));
	RenderingStep->AddBindable(Bindables::InputLayout::Resolve(VertexLayout, vShaderByteCode));


	RenderingStep->AddBindable(PixelConstantBuffer<	MaterialCommon::Buffer::ObjectLightProperties>::Resolve(m_lightProperties, MaterialCommon::Register::ObjectLightProperties));
}