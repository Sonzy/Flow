#pragma once
#include "Flow\Core.h"
#include "Flow\Rendering\Core\Materials\Material.h"
#include <DirectXMath.h>

namespace Flow
{
	class FLOW_API Mat_TexturedPhong : public Material
	{
	public:

		Mat_TexturedPhong();

		virtual void BindMaterial(Renderable* Parent, const VertexLayout& VertexLayout) override;
		virtual void BindMaterial(RenderableComponent* Parent, const VertexLayout& VertexLayout) override;



		struct ObjectLightBuffer
		{
			DirectX::XMFLOAT3 SpecularColour;
			float SpecularWeight;
			float SpecularGloss;
			float Padding[3];

		} buff;
	};
}