#pragma once
#include <string>

namespace Flow
{
	class RenderableComponent;
	class VertexLayout;
	class ShaderAsset;
	class TextureAsset;
	class Renderable;

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
		//TODO: Multiple textures/shaders etc

		TextureAsset* m_Texture;
		ShaderAsset* m_VertexShader;
		ShaderAsset* m_PixelShader;
	};
}