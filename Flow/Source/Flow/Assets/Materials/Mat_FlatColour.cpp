#include "Flowpch.h"
#include "Mat_FlatColour.h"

namespace Flow
{
	Mat_FlatColour::Mat_FlatColour()
	{
		SetVertexShader("SolidColourVS");
		SetPixelShader("SolidColourPS");
	}

	void Mat_FlatColour::BindMaterial(StaticMesh* Parent, const VertexLayout& VertexLayout)
	{
		CHECK_RETURN(!Parent, "Mat_FlatColour::BindMaterial: Parent was nullptr");

		auto vShader = VertexShader::Resolve(m_VertexShader->GetPath());
		auto vShaderByteCode = static_cast<VertexShader&>(*vShader).GetByteCode();
		Parent->AddBind(std::move(vShader));
		Parent->AddBind(PixelShader::Resolve(m_PixelShader->GetPath()));
		Parent->AddBind(InputLayout::Resolve(VertexLayout, vShaderByteCode));
	}
}

