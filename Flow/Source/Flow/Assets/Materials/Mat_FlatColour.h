#pragma once
#include "Flow\Rendering\Core\Materials\Material.h"

namespace Flow
{
	class FLOW_API Mat_FlatColour : public Material
	{
	public:

		Mat_FlatColour();

		virtual void BindMaterial(Renderable* Parent, const VertexLayout& VertexLayout) override;
		virtual void BindMaterial(RenderableComponent* Parent, const VertexLayout& VertexLayout) override;
	};
}