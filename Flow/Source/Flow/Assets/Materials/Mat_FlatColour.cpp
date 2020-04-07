#include "Flowpch.h"
#include "Mat_FlatColour.h"

#include "Flow/Rendering/Core/Bindables/Shaders/VertexShader.h"
#include "Flow/Rendering/Core/Bindables/Shaders/PixelShader.h"

#include "Flow/Rendering/Core/Mesh/StaticMesh.h"
#include "Flow\GameFramework\Components\RenderableComponent.h"


#include "Flow\Assets\AssetSystem.h"
#include "Flow\Assets\Shaders\ShaderAsset.h"
#include "Flow\Assets\Textures\TextureAsset.h"

#include "Flow\Rendering\Core\Vertex\VertexLayout.h"

namespace Flow
{
	Mat_FlatColour::Mat_FlatColour()
	{
		Colour_ = { DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) };

		SetVertexShader("SolidColourVS");
		SetPixelShader("SolidColourPS");
	}

	void Mat_FlatColour::SetColour(Vector NewColour)
	{
		Colour_ = {	DirectX::XMFLOAT4(NewColour.X, NewColour.Y, NewColour.Z, 1.0f) };
	}

	void Mat_FlatColour::BindMaterial(Renderable* Parent, const VertexLayout& VertexLayout)
	{
		CHECK_RETURN(!Parent, "Mat_FlatColour::BindMaterial: Parent was nullptr");

		auto vShader = VertexShader::Resolve(VertexShader_->GetPath());
		auto vShaderByteCode = static_cast<VertexShader&>(*vShader).GetByteCode();
		Parent->AddBind(std::move(vShader));
		Parent->AddBind(PixelShader::Resolve(PixelShader_->GetPath()));
		Parent->AddBind(InputLayout::Resolve(VertexLayout, vShaderByteCode));

		Parent->AddBind(PixelConstantBuffer<ColorBuffer>::Resolve(Colour_, 2u, GenerateTag()));
	}

	void Mat_FlatColour::BindMaterial(RenderableComponent* Parent, const VertexLayout& VertexLayout)
	{
		CHECK_RETURN(!Parent, "Mat_FlatColour::BindMaterial: Parent was nullptr");

		auto vShader = VertexShader::Resolve(VertexShader_->GetPath());
		auto vShaderByteCode = static_cast<VertexShader&>(*vShader).GetByteCode();
		Parent->AddBind(std::move(vShader));
		Parent->AddBind(PixelShader::Resolve(PixelShader_->GetPath()));
		Parent->AddBind(InputLayout::Resolve(VertexLayout, vShaderByteCode));

		Parent->AddBind(PixelConstantBuffer<ColorBuffer>::Resolve(Colour_, 2u, GenerateTag()));
	}

	std::string Mat_FlatColour::GenerateTag()
	{
		return std::string("FlatColour") +
			std::to_string(Colour_.Colour.x) +
			std::to_string(Colour_.Colour.x) +
			std::to_string(Colour_.Colour.x) +
			std::to_string(Colour_.Colour.x);
	}
}

