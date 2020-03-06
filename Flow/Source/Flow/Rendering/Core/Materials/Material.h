#pragma once
#include <string>
#include "Flow/Rendering/Core/Mesh/StaticMesh.h"
#include "Flow\Rendering\Core\Vertex\VertexLayout.h"
#include "Flow/Rendering/Core/Vertex/VertexBuffer.h"

#include "Flow\Assets\Shaders\ShaderAsset.h"
#include "Flow\Assets\Textures\TextureAsset.h"

namespace Flow
{
	class FLOW_API Material
	{
	public:
		Material();


		void BindMaterial(StaticMesh* Parent, const VertexBuffer& VB);
		virtual void BindMaterial(StaticMesh* Parent, const VertexLayout& VertexLayout);


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