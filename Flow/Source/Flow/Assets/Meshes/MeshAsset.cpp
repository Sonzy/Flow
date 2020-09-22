#include "Flowpch.h"
#include "MeshAsset.h"
#include "Flow\Application.h"

#include <Assimp/Importer.hpp>
#include <Assimp/scene.h>
#include <Assimp/postprocess.h>

#include "Flow\Assets\AssetSystem.h"

#include "Flow\Rendering\Core\Bindables\Topology.h"
#include "Flow\Rendering\Core\Bindables\BindableVertexBuffer.h"
#include "Flow/Rendering/Core/Bindables/IndexBuffer.h"

MeshAsset::MeshAsset()
{
}

MeshAsset::~MeshAsset()
{
	m_Meshes.clear();
}

bool MeshAsset::LoadAsset(const std::string& LocalPath)
{
	Assimp::Importer Importer;
	const aiScene* Scene = Importer.ReadFile(LocalPath,
		aiProcess_Triangulate | 
		aiProcess_JoinIdenticalVertices |
		aiProcess_ConvertToLeftHanded);

	CHECK_RETURN_FALSE(!Scene, "MeshAsset::LoadAsset: Failed to load model from path ({0})", LocalPath);

	//TODO: FBX doesnt load right
	aiNode* Root = Scene->mRootNode;
	for (unsigned int i = 0; i < Scene->mRootNode->mNumChildren; i++)
	{
		for (unsigned int j = 0; j < Scene->mRootNode->mChildren[i]->mNumMeshes; j++)
		{
			Mesh* NewMesh = new Mesh(this, static_cast<int>(m_Meshes.size()));

			//Load the vertices
			const aiMesh* AIMesh = Scene->mMeshes[Scene->mRootNode->mChildren[i]->mMeshes[0]];

			NewMesh->m_Vertices.reserve(AIMesh->mNumVertices);
			NewMesh->m_Normals.reserve(AIMesh->mNumVertices);
			NewMesh->m_TexCoords.reserve(AIMesh->mNumVertices);
			for (unsigned int i = 0; i < AIMesh->mNumVertices; i++)
			{
				NewMesh->m_Vertices.emplace_back(Vector3(AIMesh->mVertices[i].x, AIMesh->mVertices[i].y, AIMesh->mVertices[i].z));
				NewMesh->m_Normals.emplace_back(Vector3(AIMesh->mNormals[i].x, AIMesh->mNormals[i].y, AIMesh->mNormals[i].z));
				NewMesh->m_TexCoords.emplace_back(Vector3(AIMesh->mTextureCoords[0][i].x, AIMesh->mTextureCoords[0][i].y, AIMesh->mTextureCoords[0][i].z));
			}

			//Load the faces
			NewMesh->m_Faces.reserve(AIMesh->mNumFaces);
			for (unsigned int i = 0; i < AIMesh->mNumFaces; i++)
			{
				const auto& face = AIMesh->mFaces[i];
				NewMesh->m_Faces.emplace_back(face.mNumIndices, face.mIndices);
			}

			m_Meshes.push_back(NewMesh);
		}
	}

	GenerateAssetSize();
	m_AssetType = Asset::Type::Mesh;

	return true;
}

std::vector<Mesh*> MeshAsset::GetAllMeshes() const
{
	return m_Meshes;
}

Mesh* MeshAsset::GetMesh(int Index) const
{
	return (m_Meshes.size() > 0) ? m_Meshes[Index] : nullptr;
}

void MeshAsset::GenerateAssetSize()
{
	m_AssetSize = 0;
	for (auto& Mesh : m_Meshes)
	{
		size_t VecSize = sizeof(Vector3);
		m_AssetSize +=
			(VecSize * Mesh->m_Vertices.size()) +
			(VecSize * Mesh->m_Normals.size()) +
			(VecSize * Mesh->m_TexCoords.size()) +
			(sizeof(Mesh::Face) * Mesh->m_Faces.size());
	}
}

//= Mesh ======================

Mesh::Mesh(MeshAsset* Parent, int MeshIndex)
	: m_Parent(Parent), m_MeshIndex(MeshIndex), m_CollisionName("")
{
}

Mesh::~Mesh()
{
	m_Vertices.clear();
	m_Normals.clear();
	m_TexCoords.clear();
	m_Faces.clear();
	m_Binds.clear();
}

const std::vector<Vector3>& Mesh::GetCollisionVertices() const
{
	return !m_CollisionName.empty() ? AssetSystem::GetAsset<MeshAsset>(m_CollisionName)->GetMesh(0)->m_Vertices : m_Vertices;
}

std::vector<Mesh::Vertex> Mesh::GetVertices() const
{
	CHECK_ASSERT(m_Vertices.size() == m_Normals.size() && m_Normals.size() == m_TexCoords.size(),
		"MeshAsset::GetVertices: Property arrays are mismatched");

	std::vector<Mesh::Vertex> Vertices;
	for (int i = 0; i < m_Vertices.size(); i++)
	{
		Vertices.emplace_back(m_Vertices[i], m_Normals[i], m_TexCoords[i]);
	}

	return Vertices;
}

std::vector<std::shared_ptr<Bindable>> Mesh::GenerateBinds(VertexLayout& OutVertexLayout)
{
	//Dont regenerate if already done
	if (m_Binds.size() != 0)
	{
		OutVertexLayout = m_VertexLayout;
		return m_Binds;
	}

	m_Topology = std::dynamic_pointer_cast<Topology>(Topology::Resolve(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
	m_Binds.push_back(m_Topology);

	// Define Vertex Layout
	m_VertexLayout.Append(ElementType::Position3D);
	m_VertexLayout.Append(ElementType::Normal);
	m_VertexLayout.Append(ElementType::Texture2D);

	// Define Vertex Buffer information
	VertexBuffer VBuffer(m_VertexLayout);

	for (auto& Vertex : GetVertices())
	{
		VBuffer.EmplaceBack( //TODO: Dont actually need to know whats in here, just need to know the stride and offsets
			DirectX::XMFLOAT3{ Vertex.m_Position.x ,  Vertex.m_Position.y,  Vertex.m_Position.z },
			DirectX::XMFLOAT3{ Vertex.m_Normal.x,  Vertex.m_Normal.y,  Vertex.m_Normal.z },
			DirectX::XMFLOAT2{ Vertex.m_TexCoord.x,  Vertex.m_TexCoord.y }
		);
	}

	std::vector<unsigned short> indices;
	indices.reserve(GetNumFaces() * 3); //Using triangles, change for quads
	for (auto& Face : GetFaces())
	{
		assert(Face.m_NumIndices == 3);
		indices.push_back(Face.m_Indices[0]);
		indices.push_back(Face.m_Indices[1]);
		indices.push_back(Face.m_Indices[2]);
	}

	//Add Vertex Buffer Bind
	m_BindableVBuffer = std::dynamic_pointer_cast<BindableVertexBuffer>(BindableVertexBuffer::Resolve(m_Parent->GetAssetName() + std::to_string(m_MeshIndex), VBuffer));
	m_Binds.push_back(m_BindableVBuffer);

	//Bind Index Buffer
	assert("MeshAsset::GenerateBinds: Cannot bind multiple index buffers." && m_IndexBuffer == nullptr);
	m_IndexBuffer = std::dynamic_pointer_cast<IndexBuffer>(IndexBuffer::Resolve(m_Parent->GetAssetName() + std::to_string(m_MeshIndex), indices));
	m_Binds.push_back(m_IndexBuffer);

	OutVertexLayout = m_VertexLayout;

	return m_Binds;
}

std::shared_ptr<IndexBuffer> Mesh::GetIndexBuffer() const
{
	return m_IndexBuffer;
}

//= Struct Definitions =========================================

Mesh::Face::Face(int NumIndices, unsigned int* Indices)
	: m_NumIndices(NumIndices), m_Indices(Indices, Indices + static_cast<int>(NumIndices))
{
}

Mesh::Face::~Face()
{
	m_Indices.clear();
}