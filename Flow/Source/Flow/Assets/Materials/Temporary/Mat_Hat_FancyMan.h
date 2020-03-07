#pragma once
#include "Flow\Rendering\Core\Materials\Material.h"
#include <DirectXMath.h>

namespace Flow
{
	class FLOW_API Mat_Hat_FancyMan : public Material
	{
	public:

		Mat_Hat_FancyMan();

		virtual void BindMaterial(StaticMesh* Parent, const VertexLayout& VertexLayout) override;
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