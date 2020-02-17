#pragma once
#include <string>
#include "Flow/Rendering/Core/Mesh/StaticMesh.h"
#include "Flow\Rendering\Core\Vertex\VertexLayout.h"
#include "Flow/Rendering/Core/Vertex/VertexBuffer.h"

namespace Flow
{
	class Material
	{
	public:
		Material(const StaticMesh& Parent,
			const VertexLayout& Layout,
			VertexBuffer& VBuffer,
			const std::string& VertexShaderName,
			const std::string& PixelShaderName);

		void BindMaterial();


	protected:

		template<ElementType>
		struct VertexType{};


		template<>
		struct VertexType<ElementType::Position3D>
		{
			using DataType = DirectX::XMFLOAT3;
		};
		
	};
}

//TODO:
/*
** Material System

* Needs a shader
* Needs to get the information from the mesh and generate the vertices required to apply said shader

* Needs to automatically generate the vertex layout:
		VertexLayout Layout;
			Layout.Append(ElementType::Position3D);
			Layout.Append(ElementType::Normal);
			Layout.Append(ElementType::Texture2D);

* Needs to automatically generate the mesh vertices
						for (auto& Vertex : LoadedMesh->GetVertices())
			{
				VBuffer.EmplaceBack(
					DirectX::XMFLOAT3{ Vertex.VertexPosition.X ,  Vertex.VertexPosition.Y,  Vertex.VertexPosition.Z },
					DirectX::XMFLOAT3{ Vertex.Normal.X ,  Vertex.Normal.Y,  Vertex.Normal.Z },
					DirectX::XMFLOAT2 { Vertex.TextureCoord.X,  Vertex.TextureCoord.Y}
				);
			}
*/