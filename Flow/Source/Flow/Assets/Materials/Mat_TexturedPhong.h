#pragma once
#include "Flow\Core.h"
#include "Flow\Rendering\Core\Materials\Material.h"
#include <DirectXMath.h>

namespace Flow
{
	/* Material with lighting and a single texture. */
	class FLOW_API Mat_TexturedPhong : public Material
	{
	public:

		Mat_TexturedPhong();

		virtual void BindMaterial(Renderable* Parent, const VertexLayout& VertexLayout) override;
		virtual void BindMaterial(RenderableComponent* Parent, const VertexLayout& VertexLayout) override;

		ObjectLightBuffer LightCBuffer_;
	};
}