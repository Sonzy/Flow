#include "Flowpch.h"
#include "Material.h"

#include "Flow\Application.h"
#include "Flow/Rendering/Core/Bindables/Shaders/VertexShader.h"
#include "Flow/Rendering/Core/Bindables/Shaders/PixelShader.h"
#include "Flow\Assets\Textures\TextureAsset.h"

#include "Flow\Assets\AssetSystem.h"
#include "Flow\Assets\Shaders\ShaderAsset.h"

namespace Flow
{
	Material::Material(const StaticMesh& Parent, const VertexLayout& Layout, VertexBuffer& VBuffer,
		const std::string& VertexShaderName, const std::string& PixelShaderName)
	{
		//Vertex shader
		std::wstring VShaderPath = AssetSystem::GetAsset<ShaderAsset>(VertexShaderName)->GetPath();
		std::wstring PShaderPath = AssetSystem::GetAsset<ShaderAsset>(VertexShaderName)->GetPath();
		//pixel shader

		//constant buffers
	}


	void Material::BindMaterial()
	{
	}
}

