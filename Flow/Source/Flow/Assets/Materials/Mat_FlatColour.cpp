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
		Buffer = {
			DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f)
		};

		SetVertexShader("SolidColourVS");
		SetPixelShader("SolidColourPS");
	}

	void Mat_FlatColour::SetColour(Vector NewColour)
	{
		Buffer = {
			DirectX::XMFLOAT4(NewColour.X, NewColour.Y, NewColour.Z, 1.0f)
		};
	}

	void Mat_FlatColour::BindMaterial(Renderable* Parent, const VertexLayout& VertexLayout)
	{
		CHECK_RETURN(!Parent, "Mat_FlatColour::BindMaterial: Parent was nullptr");

		auto vShader = VertexShader::Resolve(m_VertexShader->GetPath());
		auto vShaderByteCode = static_cast<VertexShader&>(*vShader).GetByteCode();
		Parent->AddBind(std::move(vShader));
		Parent->AddBind(PixelShader::Resolve(m_PixelShader->GetPath()));
		Parent->AddBind(InputLayout::Resolve(VertexLayout, vShaderByteCode));

		Parent->AddBind(PixelConstantBuffer<ColorBuffer>::Resolve(Buffer, 2u, GenerateTag()));
	}

	void Mat_FlatColour::BindMaterial(RenderableComponent* Parent, const VertexLayout& VertexLayout)
	{
		CHECK_RETURN(!Parent, "Mat_FlatColour::BindMaterial: Parent was nullptr");

		auto vShader = VertexShader::Resolve(m_VertexShader->GetPath());
		auto vShaderByteCode = static_cast<VertexShader&>(*vShader).GetByteCode();
		Parent->AddBind(std::move(vShader));
		Parent->AddBind(PixelShader::Resolve(m_PixelShader->GetPath()));
		Parent->AddBind(InputLayout::Resolve(VertexLayout, vShaderByteCode));

		Parent->AddBind(PixelConstantBuffer<ColorBuffer>::Resolve(Buffer, 2u, GenerateTag()));
	}

	std::string Mat_FlatColour::GenerateTag()
	{
		return std::string("FlatColour") +
			std::to_string(Buffer.Colour.x) +
			std::to_string(Buffer.Colour.x) +
			std::to_string(Buffer.Colour.x) +
			std::to_string(Buffer.Colour.x);
	}
}

