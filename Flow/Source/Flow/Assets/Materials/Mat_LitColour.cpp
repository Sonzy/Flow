#include "Flowpch.h"
#include "Mat_LitColour.h"

#include "Flow\Assets\Shaders\ShaderAsset.h"
#include "Flow\Rendering\Core\Renderable.h"
#include "Flow\GameFramework\Components\RenderableComponent.h"

#include "Flow/Rendering/Core/Bindables/Shaders/VertexShader.h"
#include "Flow/Rendering/Core/Bindables/Shaders/PixelShader.h"
#include "Flow\Rendering\Core\Bindables\Sampler.h"
#include "Flow\Rendering\Core\Bindables\Texture.h"
#include "Flow\Rendering\Core\Bindables\InputLayout.h"
#include "Flow\Rendering\Core\Bindables\ConstantBuffers\ShaderConstantBuffers.h"

Flow::Mat_LitColour::Mat_LitColour()
{
	SetPixelShader("PhongColourPS");
	SetVertexShader("PhongColourVS");

	LightBuffer_ = {
		{ 0.25f , 0.5f , -1.0f },
		0.0f,
		{ 0.25f , 0.5f , -1.0f },
		0.0f,
		{ 0.2f, 0.2f, 0.2f, 1.0f },
		{ 1.0f,1.0f,1.0f, 1.0f }
	};

	Colour_ = { 
		DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
	};
}

void Flow::Mat_LitColour::SetColour(Vector NewColour)
{
	Colour_ = { DirectX::XMFLOAT4(NewColour.X, NewColour.Y, NewColour.Z, 1.0f) };

	if(ColourBuffer_)
		static_cast<PixelConstantBuffer<ColourBuffer>*>(ColourBuffer_.get())->Update(Colour_);
}

void Flow::Mat_LitColour::SetLightDirection(Vector NewDirection)
{
	LightBuffer_.Direction_ = { NewDirection.X, NewDirection.Y, NewDirection.Z };

	if (LightConstBuffer_)
		static_cast<PixelConstantBuffer<DirectionalLightData>*>(LightConstBuffer_.get())->Update(LightBuffer_);
}

void Flow::Mat_LitColour::SetRotated(bool Rotate)
{
	//LightBuffer_.UseRotated = Rotate;

	//if (LightConstBuffer_)
	//	static_cast<PixelConstantBuffer<DirectionalLightData>*>(LightConstBuffer_.get())->Update(LightBuffer_);
}

Vector Flow::Mat_LitColour::GetLightDirection(Vector NewDirection)
{
	return Vector(LightBuffer_.Direction_.x, LightBuffer_.Direction_.y, LightBuffer_.Direction_.z);
}

std::string Flow::Mat_LitColour::GenerateTag()
{
	return std::string("LitColour") +
		std::to_string(Colour_.Colour_.x) +
		std::to_string(Colour_.Colour_.y) +
		std::to_string(Colour_.Colour_.z) +
		std::to_string(Colour_.Colour_.w);
}

void Flow::Mat_LitColour::BindMaterial(Renderable* Parent, const VertexLayout& VertexLayout)
{
	CHECK_RETURN(!Parent, "Mat_FlatColour::BindMaterial: Parent was nullptr");

	auto vShader = VertexShader::Resolve(VertexShader_->GetPath());
	auto vShaderByteCode = static_cast<VertexShader&>(*vShader).GetByteCode();
	Parent->AddBind(std::move(vShader));
	Parent->AddBind(PixelShader::Resolve(PixelShader_->GetPath()));
	Parent->AddBind(InputLayout::Resolve(VertexLayout, vShaderByteCode));

	ColourBuffer_ = PixelConstantBuffer<ColourBuffer>::Resolve(Colour_, 2u, GenerateTag());
	LightConstBuffer_ = PixelConstantBuffer<DirectionalLightData>::Resolve(LightBuffer_, 1u);
	Parent->AddBind(LightConstBuffer_);
	Parent->AddBind(ColourBuffer_);
}

void Flow::Mat_LitColour::BindMaterial(RenderableComponent* Parent, const VertexLayout& VertexLayout)
{
	CHECK_RETURN(!Parent, "Mat_FlatColour::BindMaterial: Parent was nullptr");

	auto vShader = VertexShader::Resolve(VertexShader_->GetPath());
	auto vShaderByteCode = static_cast<VertexShader&>(*vShader).GetByteCode();
	Parent->AddBind(std::move(vShader));
	Parent->AddBind(PixelShader::Resolve(PixelShader_->GetPath()));
	Parent->AddBind(InputLayout::Resolve(VertexLayout, vShaderByteCode));

	ColourBuffer_ = PixelConstantBuffer<ColourBuffer>::Resolve(Colour_, 2u, GenerateTag());
	LightConstBuffer_ = PixelConstantBuffer<DirectionalLightData>::Resolve(LightBuffer_, 1u);
	Parent->AddBind(LightConstBuffer_);
	Parent->AddBind(ColourBuffer_);
}
