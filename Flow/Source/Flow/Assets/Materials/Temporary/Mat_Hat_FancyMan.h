#pragma once
#include "Flow\Rendering\Core\Materials\Material.h"

namespace Flow
{
	class FLOW_API Mat_Hat_FancyMan : public Material
	{
	public:

		Mat_Hat_FancyMan();

		virtual void BindMaterial(StaticMesh* Parent, const std::vector<D3D11_INPUT_ELEMENT_DESC>& VertexLayout) override;
	};
}