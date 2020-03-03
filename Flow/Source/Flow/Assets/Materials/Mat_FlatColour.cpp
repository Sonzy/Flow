#include "Flowpch.h"
#include "Mat_FlatColour.h"

namespace Flow
{
	Mat_FlatColour::Mat_FlatColour()
	{
		SetVertexShader("SolidColourVS");
		SetPixelShader("SolidColourPS");
	}

	void Mat_FlatColour::BindMaterial(StaticMesh* Parent, const std::vector<D3D11_INPUT_ELEMENT_DESC>& VertexLayout)
	{
		CHECK_RETURN(!Parent, "Mat_FlatColour::BindMaterial: Parent was nullptr");

		auto vShader = std::make_unique<VertexShader>(m_VertexShader->GetPath());
		auto vShaderByteCode = vShader->GetByteCode();
		Parent->ReplaceOrAddStaticBindable<VertexShader>(std::move(vShader));
		Parent->ReplaceOrAddStaticBindable<PixelShader>(std::make_unique<PixelShader>(m_PixelShader->GetPath()));
		Parent->ReplaceOrAddStaticBindable<InputLayout>(std::make_unique<InputLayout>(VertexLayout, vShaderByteCode));

		if (Parent->StaticInitialised())
			Parent->RefreshStaticBinds();
	}
}

