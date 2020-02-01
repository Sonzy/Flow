#include "Flowpch.h"
#include "StaticMesh.h"
#include "Flow/Rendering/Core/Vertex/VertexLayout.h"
#include "Flow/Rendering/Core/Vertex/VertexBuffer.h"

#include <Assimp/Importer.hpp>
#include <Assimp/scene.h>
#include <Assimp/postprocess.h>

namespace Flow
{
	StaticMesh::StaticMesh(const std::string& LocalPath)
	{
		// Define Vertex Layout
		VertexLayout Layout;
		Layout.Append(ElementType::Position3D);
		
		// Define Vertex Buffer information
		VertexBuffer VBuffer(Layout);
		
		//Import Model
		Assimp::Importer Importer;
		const aiScene* Model = Importer.ReadFile(LocalPath,
			aiProcess_Triangulate |
			aiProcess_JoinIdenticalVertices);

		if (!Model)
		{
			FLOW_ENGINE_ERROR("{0}", Importer.GetErrorString());
			return;
		}

		
		//CHECK_RETURN(!Model, "StaticMesh::StaticMesh: Failed to load model");

		//Load the data from each vertex
		const auto Mesh = Model->mMeshes[0];
		for(unsigned int i = 0; i < Mesh->mNumVertices; i++)
		{
			VBuffer.EmplaceBack(
				DirectX::XMFLOAT3{ Mesh->mVertices[i].x, Mesh->mVertices[i].y, Mesh->mVertices[i].z }
			);
		}
		
		//Load the indices
		std::vector<unsigned short> indices;
		indices.reserve(Mesh->mNumFaces * 3); //Using triangles, change for quads
		for (unsigned int i = 0; i < Mesh->mNumFaces; i++)
		{
			const auto& face = Mesh->mFaces[i];
			assert(face.mNumIndices == 3);
			indices.push_back(face.mIndices[0]);
			indices.push_back(face.mIndices[1]);
			indices.push_back(face.mIndices[2]);
		}

		//Add Vertex Buffer Bind
		AddStaticBindable(std::make_unique<BindableVertexBuffer>(VBuffer));
		
		//Bind Shaders
		auto vShader = std::make_unique<VertexShader>(L"Source/Shaders/SolidColourVS.cso");
		auto vShaderByteCode = vShader->GetByteCode();
		AddStaticBindable(std::move(vShader));
		AddStaticBindable(std::make_unique<PixelShader>(L"Source/Shaders/SolidColourPS.cso"));
		
		//Bind Index Buffer
		AddStaticIndexBuffer(std::make_unique<IndexBuffer>(indices));
		
		//Bind Input Layout
		AddStaticBindable(std::make_unique<InputLayout>(VBuffer.GetLayout().GetD3DLayout(), vShaderByteCode));
		
		//Bind Transform
		AddStaticBindable(std::make_unique<TransformConstantBuffer>(this));
	}
	DirectX::XMMATRIX StaticMesh::GetTransformXM() const
	{
		return DirectX::XMMATRIX();
	}
}