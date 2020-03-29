#include "Flowpch.h"
#include "MeshAsset.h"


#include "Flow\Application.h"

#include <Assimp/Importer.hpp>
#include <Assimp/scene.h>
#include <Assimp/postprocess.h>

#include "Flow\Assets\AssetSystem.h"

	//MeshAsset::~MeshAsset()
	//{
	//	m_Vertices.clear();
	//	m_Normals.clear();
	//	m_TexCoords.clear();
	//	m_Faces.clear();
	//}
	//
	//bool MeshAsset::LoadAsset(const std::string& LocalPath)
	//{
	//	Assimp::Importer Importer;
	//	const aiScene* Model = Importer.ReadFile(Application::GetApplication().GetLocalFilePath() + LocalPath,
	//		aiProcess_Triangulate |
	//		aiProcess_JoinIdenticalVertices |
	//		aiProcess_ConvertToLeftHanded);
	//
	//	if (!Model)
	//		return false;
	//
	//	//Load the vertices
	//	const aiMesh* Mesh = Model->mMeshes[0];
	//	for (unsigned int i = 0; i < Mesh->mNumVertices; i++)
	//	{
	//		m_Vertices.emplace_back( Vector(Mesh->mVertices[i].x, Mesh->mVertices[i].y, Mesh->mVertices[i].z) );
	//		m_Normals.emplace_back(Vector(Mesh->mNormals[i].x, Mesh->mNormals[i].y, Mesh->mNormals[i].z));
	//		m_TexCoords.emplace_back(Vector(Mesh->mTextureCoords[0][i].x, Mesh->mTextureCoords[0][i].y, Mesh->mTextureCoords[0][i].z));
	//	}
	//
	//	//Load the faces
	//	m_Faces.reserve(Mesh->mNumFaces);
	//	for (unsigned int i = 0; i < Mesh->mNumFaces; i++)
	//	{
	//		//TODO: Check that the mesh is actually triangles instead of quads
	//		const auto& face = Mesh->mFaces[i];
	//		m_Faces.emplace_back(face.mNumIndices, face.mIndices);
	//	}
	//
	//	GenerateAssetSize();
	//	m_AssetType = EAssetType::Mesh;
	//
	//	return true;
	//}
	//
	//std::vector<MeshVertex> MeshAsset::GetVertices() const
	//{
	//	CHECK_ASSERT(m_Vertices.size() == m_Normals.size() && m_Normals.size() == m_TexCoords.size(),
	//		"MeshAsset::GetVertices: Property arrays are mismatched");
	//
	//	std::vector<MeshVertex> Vertices;
	//	for (int i = 0; i < m_Vertices.size(); i++)
	//	{
	//		Vertices.emplace_back(m_Vertices[i], m_Normals[i], m_TexCoords[i]);
	//	}
	//
	//	return Vertices;
	//}
	//
	//void MeshAsset::SetMaterial(Material* NewMaterial)
	//{
	//	m_Material = NewMaterial;
	//}
	//
	//std::vector<std::shared_ptr<Bindable>> MeshAsset::GenerateBinds(VertexLayout& OutVertexLayout)
	//{
	//	//Dont regenerate if already done
	//	if (m_Binds.size() != 0)
	//	{
	//		OutVertexLayout = m_VertexLayout;
	//		return m_Binds;
	//	}
	//
	//	m_Binds.push_back(Topology::Resolve(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
	//
	//	// Define Vertex Layout
	//	m_VertexLayout.Append(ElementType::Position3D);
	//	m_VertexLayout.Append(ElementType::Normal);
	//	m_VertexLayout.Append(ElementType::Texture2D);
	//
	//	// Define Vertex Buffer information
	//	VertexBuffer VBuffer(m_VertexLayout);
	//
	//	for (auto& Vertex : GetVertices())
	//	{
	//		VBuffer.EmplaceBack( //TODO: Dont actually need to know whats in here, just need to know the stride and offsets
	//			DirectX::XMFLOAT3{ Vertex.Position.X ,  Vertex.Position.Y,  Vertex.Position.Z },
	//			DirectX::XMFLOAT3{ Vertex.Normal.X ,  Vertex.Normal.Y,  Vertex.Normal.Z },
	//			DirectX::XMFLOAT2{ Vertex.TexCoord.X,  Vertex.TexCoord.Y }
	//		);
	//	}
	//
	//	std::vector<unsigned short> indices;
	//	indices.reserve(GetNumFaces() * 3); //Using triangles, change for quads
	//	for (auto& Face : GetFaces())
	//	{
	//		assert(Face.m_NumIndices == 3);
	//		indices.push_back(Face.m_Indices[0]);
	//		indices.push_back(Face.m_Indices[1]);
	//		indices.push_back(Face.m_Indices[2]);
	//	}
	//
	//	//Add Vertex Buffer Bind
	//	m_Binds.push_back(BindableVertexBuffer::Resolve(m_AssetName, VBuffer));
	//
	//	//Bind Index Buffer
	//	std::shared_ptr<Bindable> Index = IndexBuffer::Resolve(m_AssetName, indices);
	//	assert("MeshAsset::GenerateBinds: Cannot bind multiple index buffers." && m_IndexBuffer == nullptr);
	//	m_Binds.push_back(Index);
	//	m_IndexBuffer = static_cast<IndexBuffer*>(Index.get());
	//
	//	OutVertexLayout = m_VertexLayout;
	//
	//	return m_Binds;
	//}
	//
	//const IndexBuffer* MeshAsset::GetIndexBuffer() const
	//{
	//	return m_IndexBuffer;
	//}
	//
	//void MeshAsset::GenerateAssetSize()
	//{
	//	size_t VecSize = sizeof(Vector);
	//	m_AssetSize = 
	//		(VecSize * m_Vertices.size()) + 
	//		(VecSize * m_Normals.size()) +
	//		(VecSize * m_TexCoords.size()) +
	//		(sizeof(MeshFace) * m_Faces.size());
	//}

namespace Flow
{
	MeshAsset::MeshAsset()
	{
	}
	MeshAsset::~MeshAsset()
	{
	}

	bool MeshAsset::LoadAsset(const std::string& LocalPath)
	{
		Assimp::Importer Importer;
		const aiScene* Scene = Importer.ReadFile(Application::GetApplication().GetLocalFilePath() + LocalPath,
			aiProcess_Triangulate |
			aiProcess_JoinIdenticalVertices |
			aiProcess_ConvertToLeftHanded);

		CHECK_RETURN_FALSE(!Scene, "MeshAsset::LoadAsset: Failed to load model from path ({0})", LocalPath);

		//TODO: FBX doesnt load right
		aiNode* Root = Scene->mRootNode;
		for (int i = 0; i < Scene->mRootNode->mNumChildren; i++)
		{
			for (int j = 0; j < Scene->mRootNode->mChildren[i]->mNumMeshes; j++)
			{
				Mesh* NewMesh = new Mesh(this, m_Meshes.size());

				//Load the vertices
				const aiMesh* AIMesh = Scene->mMeshes[Scene->mRootNode->mChildren[i]->mMeshes[0]];
				for (unsigned int i = 0; i < AIMesh->mNumVertices; i++)
				{
					NewMesh->m_Vertices.emplace_back(Vector(AIMesh->mVertices[i].x, AIMesh->mVertices[i].y, AIMesh->mVertices[i].z));
					NewMesh->m_Normals.emplace_back(Vector(AIMesh->mNormals[i].x, AIMesh->mNormals[i].y, AIMesh->mNormals[i].z));
					NewMesh->m_TexCoords.emplace_back(Vector(AIMesh->mTextureCoords[0][i].x, AIMesh->mTextureCoords[0][i].y, AIMesh->mTextureCoords[0][i].z));
				}

				//Load the faces
				NewMesh->m_Faces.reserve(AIMesh->mNumFaces);
				for (unsigned int i = 0; i < AIMesh->mNumFaces; i++)
				{
					//TODO: Check that the mesh is actually triangles instead of quads
					const auto& face = AIMesh->mFaces[i];
					NewMesh->m_Faces.emplace_back(face.mNumIndices, face.mIndices);
				}

				m_Meshes.push_back(NewMesh);
			}
		}

		//Load all meshes
		//for (int i = 0; i < Scene->mNumMeshes; i++)
		//{
		//	Mesh* NewMesh = new Mesh(this, m_Meshes.size());
		//
		//	//Load the vertices
		//	const aiMesh* AIMesh = Scene->mMeshes[0];
		//	for (unsigned int i = 0; i < AIMesh->mNumVertices; i++)
		//	{
		//		NewMesh->m_Vertices.emplace_back(Vector(AIMesh->mVertices[i].x, AIMesh->mVertices[i].y, AIMesh->mVertices[i].z));
		//		NewMesh->m_Normals.emplace_back(Vector(AIMesh->mNormals[i].x, AIMesh->mNormals[i].y, AIMesh->mNormals[i].z));
		//		NewMesh->m_TexCoords.emplace_back(Vector(AIMesh->mTextureCoords[0][i].x, AIMesh->mTextureCoords[0][i].y, AIMesh->mTextureCoords[0][i].z));
		//	}
		//
		//	//Load the faces
		//	NewMesh->m_Faces.reserve(AIMesh->mNumFaces);
		//	for (unsigned int i = 0; i < AIMesh->mNumFaces; i++)
		//	{
		//		//TODO: Check that the mesh is actually triangles instead of quads
		//		const auto& face = AIMesh->mFaces[i];
		//		NewMesh->m_Faces.emplace_back(face.mNumIndices, face.mIndices);
		//	}
		//
		//	m_Meshes.push_back(NewMesh);
		//}

		GenerateAssetSize();
		m_AssetType = EAssetType::MultiMesh;

		return true;
	}

	std::vector<Mesh*> MeshAsset::GetAllMeshes() const
	{
		return m_Meshes;
	}

	Mesh* MeshAsset::GetMesh(int Index) const
	{
		return m_Meshes[Index];
	}

	void MeshAsset::GenerateAssetSize()
	{
		m_AssetSize = 0;
		for (auto& Mesh : m_Meshes)
		{
			size_t VecSize = sizeof(Vector);
			m_AssetSize +=
				(VecSize * Mesh->m_Vertices.size()) +
				(VecSize * Mesh->m_Normals.size()) +
				(VecSize * Mesh->m_TexCoords.size()) +
				(sizeof(MeshFace) * Mesh->m_Faces.size());
		}
	}

	//= Mesh ======================

	Mesh::Mesh(MeshAsset* Parent, int MeshIndex)
		: m_Parent(Parent), m_MeshIndex(MeshIndex), CollisionName_("")
	{
	}

	const std::vector<Vector>& Mesh::GetCollisionVertices() const
	{
		return !CollisionName_.empty() ? AssetSystem::GetAsset<MeshAsset>(CollisionName_)->GetMesh(0)->m_Vertices : m_Vertices;
	}

	std::vector<MeshVertex> Mesh::GetVertices() const
	{
		CHECK_ASSERT(m_Vertices.size() == m_Normals.size() && m_Normals.size() == m_TexCoords.size(),
			"MeshAsset::GetVertices: Property arrays are mismatched");

		std::vector<MeshVertex> Vertices;
		for (int i = 0; i < m_Vertices.size(); i++)
		{
			Vertices.emplace_back(m_Vertices[i], m_Normals[i], m_TexCoords[i]);
		}

		return Vertices;
	}

	void Mesh::SetMaterial(Material* NewMaterial)
	{
		m_Material = NewMaterial;
	}

	std::vector<std::shared_ptr<Bindable>> Mesh::GenerateBinds(VertexLayout& OutVertexLayout)
	{
		//Dont regenerate if already done
		if (m_Binds.size() != 0)
		{
			OutVertexLayout = m_VertexLayout;
			return m_Binds;
		}

		m_Binds.push_back(Topology::Resolve(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

		// Define Vertex Layout
		m_VertexLayout.Append(ElementType::Position3D);
		m_VertexLayout.Append(ElementType::Normal);
		m_VertexLayout.Append(ElementType::Texture2D);

		// Define Vertex Buffer information
		VertexBuffer VBuffer(m_VertexLayout);

		for (auto& Vertex : GetVertices())
		{
			VBuffer.EmplaceBack( //TODO: Dont actually need to know whats in here, just need to know the stride and offsets
				DirectX::XMFLOAT3{ Vertex.Position.X ,  Vertex.Position.Y,  Vertex.Position.Z },
				DirectX::XMFLOAT3{ Vertex.Normal.X ,  Vertex.Normal.Y,  Vertex.Normal.Z },
				DirectX::XMFLOAT2{ Vertex.TexCoord.X,  Vertex.TexCoord.Y }
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
		m_Binds.push_back(BindableVertexBuffer::Resolve(m_Parent->GetAssetName() + std::to_string(m_MeshIndex), VBuffer));

		//Bind Index Buffer
		std::shared_ptr<Bindable> Index = IndexBuffer::Resolve(m_Parent->GetAssetName() + std::to_string(m_MeshIndex), indices);
		assert("MeshAsset::GenerateBinds: Cannot bind multiple index buffers." && m_IndexBuffer == nullptr);
		m_Binds.push_back(Index);
		m_IndexBuffer = static_cast<IndexBuffer*>(Index.get());

		OutVertexLayout = m_VertexLayout;

		return m_Binds;
	}

	const IndexBuffer* Mesh::GetIndexBuffer() const
	{
		return m_IndexBuffer;
	}
}