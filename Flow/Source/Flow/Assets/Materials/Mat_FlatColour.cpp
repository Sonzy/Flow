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

Mat_FlatColour::Mat_FlatColour()
{
	_Colour = { DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) };

	SetVertexShader("SolidColourVS");
	SetPixelShader("SolidColourPS");
}

void Mat_FlatColour::SetColour(Vector NewColour)
{
	_Colour = { DirectX::XMFLOAT4(NewColour.X, NewColour.Y, NewColour.Z, 1.0f) };
}

void Mat_FlatColour::BindMaterial(Step* RenderingStep, const VertexLayout& VertexLayout)
{
	auto vShader = VertexShader::Resolve(_VertexShader->GetPath());
	auto vShaderByteCode = static_cast<VertexShader&>(*vShader).GetByteCode();
	RenderingStep->AddBindable(std::move(vShader));
	RenderingStep->AddBindable(PixelShader::Resolve(_PixelShader->GetPath()));
	RenderingStep->AddBindable(InputLayout::Resolve(VertexLayout, vShaderByteCode));

	RenderingStep->AddBindable(PixelConstantBuffer<ColorBuffer>::Resolve(_Colour, 2u, GenerateTag()));
}

std::string Mat_FlatColour::GenerateTag()
{
	return std::string("FlatColour") +
		std::to_string(_Colour.Colour.x) +
		std::to_string(_Colour.Colour.x) +
		std::to_string(_Colour.Colour.x) +
		std::to_string(_Colour.Colour.x);
}

