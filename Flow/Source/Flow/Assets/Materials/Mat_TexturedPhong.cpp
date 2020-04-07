#include "Flowpch.h"
#include "Mat_TexturedPhong.h"

#include "Flow\Helper\HelperMacros.h"
#include "Flow\Logging\Log.h"

#include "Flow\Rendering\Core\Renderable.h"
#include "Flow\GameFramework\Components\RenderableComponent.h"

#include "Flow/Rendering/Core/Bindables/Shaders/VertexShader.h"
#include "Flow/Rendering/Core/Bindables/Shaders/PixelShader.h"
#include "Flow\Rendering\Core\Bindables\Sampler.h"
#include "Flow\Rendering\Core\Bindables\Texture.h"
#include "Flow\Rendering\Core\Bindables\InputLayout.h"
#include "Flow\Rendering\Core\Bindables\ConstantBuffers\ShaderConstantBuffers.h"

#include "Flow\Assets\Shaders\ShaderAsset.h"

namespace Flow
{
	Mat_TexturedPhong::Mat_TexturedPhong()
	{
		SetTexture("Wabble_Props");
		SetPixelShader("TexturedPhongPS");
		SetVertexShader("TexturedPhongVS");

		LightCBuffer_ =
		{
			{1.0f, 1.0f, 1.0f},
			0.1f,
			20.0f,
			{0.0f, 0.0f, 0.0f}
		};
	}

	void Mat_TexturedPhong::BindMaterial(Renderable* Parent, const VertexLayout& VertexLayout)
	{
		CHECK_RETURN(!Parent, "Mat_FlatColour::BindMaterial: Parent was nullptr");

		Parent->AddBind(Texture::Resolve(Texture_, 0));
		Parent->AddBind(Sampler::Resolve());
		auto vShader = VertexShader::Resolve(VertexShader_->GetPath());
		auto vShaderByteCode = static_cast<VertexShader&>(*vShader).GetByteCode();
		Parent->AddBind(std::move(vShader));
		Parent->AddBind(PixelShader::Resolve(PixelShader_->GetPath()));
		Parent->AddBind(InputLayout::Resolve(VertexLayout, vShaderByteCode));


		Parent->AddBind(PixelConstantBuffer<ObjectLightBuffer>::Resolve(LightCBuffer_, 1u));
	}

	void Mat_TexturedPhong::BindMaterial(RenderableComponent* Parent, const VertexLayout& VertexLayout)
	{
		CHECK_RETURN(!Parent, "Mat_FlatColour::BindMaterial: Parent was nullptr");

		Parent->AddBind(Texture::Resolve(Texture_, 0));
		Parent->AddBind(Sampler::Resolve());
		auto vShader = VertexShader::Resolve(VertexShader_->GetPath());
		auto vShaderByteCode = static_cast<VertexShader&>(*vShader).GetByteCode();
		Parent->AddBind(std::move(vShader));
		Parent->AddBind(PixelShader::Resolve(PixelShader_->GetPath()));
		Parent->AddBind(InputLayout::Resolve(VertexLayout, vShaderByteCode));


		Parent->AddBind(PixelConstantBuffer<ObjectLightBuffer>::Resolve(LightCBuffer_, 1u));
	}
}