#include "Material.h"

#include "Flow\Application.h"
#include "Flow/Rendering/Core/Bindables/Shaders/VertexShader.h"
#include "Flow/Rendering/Core/Bindables/Shaders/PixelShader.h"
#include "Flow\Assets\Textures\TextureAsset.h"

namespace Flow
{
	Material::Material(const StaticMesh& Parent, const VertexLayout& Layout, VertexBuffer& VBuffer,
		const std::string& VertexShaderName, const std::string& PixelShaderName)
	{
		size_t Count = Layout.GetElementCount();
		for (size_t i = 0; i < Count; i++)
		{
			Element El = Layout.GetElementByIndex(i);


			switch (El.GetType())
			{
			case ElementType::Position3D:

				VertexType<ElementType::Position3D>::DataType F3 = {0.0f,0.0f, 0.0f};

				break;
			case ElementType::Normal:
				break;
			case ElementType::Texture2D:
				break;
			default:
				break;
			}	



			//VBuffer.EmplaceBack(
			//
			//)
		}


		//Vertex shader

		//pixel shader

		//input layout

		//constant buffers
	}


	void Material::BindMaterial()
	{
	}
}

