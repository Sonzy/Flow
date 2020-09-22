#include "Flowpch.h"
#include "Mat_TexturedPhong.h"

#include "Rendering/Core/Bindables/Shaders/VertexShader.h"
#include "Rendering/Core/Bindables/Shaders/PixelShader.h"
#include "Rendering/Core/Bindables/Sampler.h"
#include "Rendering/Core/Bindables/Texture.h"
#include "Rendering/Core/Bindables/InputLayout.h"
#include "Rendering/Core/Bindables/ConstantBuffers/ShaderConstantBuffers.h"

#include "Assets/Shaders/ShaderAsset.h"

Mat_TexturedPhong::Mat_TexturedPhong()
{
	SetTexture("Wabble_Props");
	SetPixelShader("TexturedPhongPS");
	SetVertexShader("TexturedPhongVS");

	m_LightCBuffer =
	{
		{1.0f, 1.0f, 1.0f},
		0.1f,
		20.0f,
		{0.0f, 0.0f, 0.0f}
	};
}

void Mat_TexturedPhong::BindMaterial(Step* RenderingStep, const VertexLayout& VertexLayout)
{
	RenderingStep->AddBindable(Texture::Resolve(_Texture, 0));
	RenderingStep->AddBindable(Sampler::Resolve());
	auto vShader = VertexShader::Resolve(_VertexShader->GetPath());
	auto vShaderByteCode = static_cast<VertexShader&>(*vShader).GetByteCode();
	RenderingStep->AddBindable(std::move(vShader));
	RenderingStep->AddBindable(PixelShader::Resolve(_PixelShader->GetPath()));
	RenderingStep->AddBindable(InputLayout::Resolve(VertexLayout, vShaderByteCode));


	RenderingStep->AddBindable(PixelConstantBuffer<ObjectLightBuffer>::Resolve(m_LightCBuffer, 1u));
}