#pragma once
#include "Flow\Core.h"
#include "Flow\Rendering\Core\Materials\Material.h"
#include "Flow\Helper\Maths.h"

#include "Flow\Rendering\Core\Bindables\ConstantBuffers\ShaderConstantBuffers.h"

namespace Flow
{
	/* Material with lighting and a single colour. */
	class FLOW_API Mat_LitColour : public Material
	{
	public:

		Mat_LitColour();

		virtual void BindMaterial(Renderable* Parent, const VertexLayout& VertexLayout) override;
		virtual void BindMaterial(RenderableComponent* Parent, const VertexLayout& VertexLayout) override;

		void SetColour(Vector NewColour);
		void SetLightDirection(Vector NewDirection);
		void SetRotated(bool Rotate);
		Vector GetLightDirection(Vector NewDirection);

		struct DirectionalLightData
		{
			DirectX::XMFLOAT3 Direction_;
			float padding;
			DirectX::XMFLOAT3 RotatedDir;
			float padding2;
			DirectX::XMFLOAT4 Ambient_;
			DirectX::XMFLOAT4 DiffuseColor_;
		} LightBuffer_;

		struct ColourBuffer
		{
			DirectX::XMFLOAT4 Colour_;
		} Colour_;

		std::shared_ptr<Bindable> ColourBuffer_;
		std::shared_ptr<Bindable> LightConstBuffer_;

		std::string GenerateTag();
	};
}