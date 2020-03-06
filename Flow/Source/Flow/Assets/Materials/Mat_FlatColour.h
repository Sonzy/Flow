#pragma once
#include "Flow\Rendering\Core\Materials\Material.h"

namespace Flow
{
	class FLOW_API Mat_FlatColour : public Material
	{
	public:

		Mat_FlatColour();

		virtual void BindMaterial(StaticMesh* Parent, const VertexLayout& VertexLayout) override;
	};
}