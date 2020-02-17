#pragma once
#include <string>
#include "Flow/Rendering/Core/Mesh/StaticMesh.h"
#include "Flow\Rendering\Core\Vertex\VertexLayout.h"
#include "Flow/Rendering/Core/Vertex/VertexBuffer.h"

#include "Flow\Assets\Shaders\ShaderAsset.h"
#include "Flow\Assets\Textures\TextureAsset.h"

namespace Flow
{
	class Material
	{
	public:
		Material(StaticMesh* Parent);

		void BindMaterial(const VertexBuffer& VB);
		void SetTexture(const std::string& TextureName);
		void SetPixelShader(const std::string& ShaderName);
		void SetVertexShader(const std::string& ShaderName);

	protected:

		StaticMesh* m_Parent;
		//TODO: Multiple textures/shaders etc

		TextureAsset* m_Texture;
		ShaderAsset* m_VertexShader;
		ShaderAsset* m_PixelShader;
	};
}