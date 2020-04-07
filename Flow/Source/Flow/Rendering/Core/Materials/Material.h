#pragma once
#include <string>
#include <DirectXMath.h>

namespace Flow
{
	class RenderableComponent;
	class VertexLayout;
	class ShaderAsset;
	class TextureAsset;
	class Renderable;

	/* Base class for materials. Allows for easy setting of textures and shaders. To be extended in the future for dynamic materials */
	class FLOW_API Material
	{
	public:
		Material();

		virtual void BindMaterial(Renderable* Parent, const VertexLayout& VertexLayout);
		virtual void BindMaterial(RenderableComponent* Parent, const VertexLayout& VertexLayout);


		void SetTexture(const std::string& TextureName);
		void SetPixelShader(const std::string& ShaderName);
		void SetVertexShader(const std::string& ShaderName);

	protected:
		//TODO: Multiple textures etc

		TextureAsset* Texture_;
		ShaderAsset* VertexShader_;
		ShaderAsset* PixelShader_;
	};

	/* Struct used for sending light data vis */
	struct ObjectLightBuffer
	{
		DirectX::XMFLOAT3 SpecularColour_;
		float SpecularWeight_;
		float SpecularGloss_;
		float Padding_[3];
	};
}