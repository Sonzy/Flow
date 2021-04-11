#include "pch.h"
#include "MeshAsset.h"
#include "Application.h"

#include <Assimp/Importer.hpp>
#include <Assimp/scene.h>
#include <Assimp/postprocess.h>

#include "Assets/AssetSystem.h"

#include "Rendering/Core/Bindables/Topology.h"
#include "Rendering/Core/Bindables/VertexBuffer.h"
#include "Rendering/Core/Bindables/IndexBuffer.h"

#include "Typedefs.h"

MeshAsset::MeshAsset()
{
}

MeshAsset::~MeshAsset()
{
	m_Meshes.clear();
}

bool MeshAsset::ImportAsset(const std::string& FilePath, const std::string& SavePath)
{
	//TODO: Testing local saving
	std::filesystem::path Converted = FilePath;
	Converted.replace_extension(".fMesh");
	if (std::filesystem::exists(Converted))
	{
		return LoadAsset(Converted.string());
	}

	Assimp::Importer Importer;
	const aiScene* Scene = Importer.ReadFile(FilePath,
		aiProcess_Triangulate | 
		aiProcess_JoinIdenticalVertices |
		aiProcess_ConvertToLeftHanded);
	
	CHECK_RETURN_FALSE(!Scene, ("MeshAsset::LoadAsset: Failed to load model from path ({0}) {1}", FilePath, Importer.GetErrorString()));

	//TODO: FBX doesnt load right
	aiNode* Root = Scene->mRootNode;
	for (unsigned int i = 0; i < Scene->mRootNode->mNumChildren; i++)
	{
		for (unsigned int j = 0; j < Scene->mRootNode->mChildren[i]->mNumMeshes; j++)
		{
			Mesh NewMesh = Mesh(this, static_cast<int>(m_Meshes.size()));

			//Load the vertices
			const aiMesh* AIMesh = Scene->mMeshes[Scene->mRootNode->mChildren[i]->mMeshes[0]];

			NewMesh.m_Vertices.reserve(AIMesh->mNumVertices);
			NewMesh.m_Normals.reserve(AIMesh->mNumVertices);
			NewMesh.m_TexCoords.reserve(AIMesh->mNumVertices);
			for (unsigned int i = 0; i < AIMesh->mNumVertices; i++)
			{
				NewMesh.m_Vertices.emplace_back(Vector3(AIMesh->mVertices[i].x, AIMesh->mVertices[i].y, AIMesh->mVertices[i].z));
				NewMesh.m_Normals.emplace_back(Vector3(AIMesh->mNormals[i].x, AIMesh->mNormals[i].y, AIMesh->mNormals[i].z));
				NewMesh.m_TexCoords.emplace_back(Vector3(AIMesh->mTextureCoords[0][i].x, AIMesh->mTextureCoords[0][i].y, AIMesh->mTextureCoords[0][i].z));
			}

			//Load the faces
			NewMesh.m_Faces.reserve(AIMesh->mNumFaces);
			for (unsigned int i = 0; i < AIMesh->mNumFaces; i++)
			{
				const auto& face = AIMesh->mFaces[i];
				NewMesh.m_Faces.emplace_back(face.mNumIndices, face.mIndices);
			}

			m_Meshes.push_back(NewMesh);
		}
	}

	GenerateAssetSize();
	m_AssetType = Asset::Type::Mesh;

	//= Save the imported asset =

	fs::path ConvertedPath = SavePath;
	ConvertedPath.replace_extension(".fmesh");
	SaveAsset(ConvertedPath.string());

	return true;
}

bool MeshAsset::SaveAsset(const std::string& AssetName)
{
	std::ofstream OutStream = std::ofstream(AssetName, std::ios::out | std::ios::trunc | std::ios::binary);

	size_t buffer;

	// MeshCount
	buffer = m_Meshes.size();
	OutStream.write(reinterpret_cast<const char*>(&buffer), sizeof(size_t));

	for (const Mesh& mesh : m_Meshes)
	{
		// Collision Name
		OutStream.write(mesh.m_CollisionName.data(), sizeof(char) * 64);

		// Number of Vertices
		buffer = mesh.m_Vertices.size();
		OutStream.write(reinterpret_cast<const char*>(&buffer), sizeof(size_t));

		// Number of Faces
		buffer = mesh.m_Faces.size();
		OutStream.write(reinterpret_cast<const char*>(&buffer), sizeof(size_t));

		for (int i = 0; i < mesh.m_Vertices.size(); i++)
		{
			// Write Vertex position
			OutStream.write(reinterpret_cast<const char*>(&mesh.m_Vertices[i]), sizeof(Vector3));

			// Write Normal
			OutStream.write(reinterpret_cast<const char*>(&mesh.m_Normals[i]), sizeof(Vector3));

			// Write TexCoord
			OutStream.write(reinterpret_cast<const char*>(&mesh.m_TexCoords[i]), sizeof(Vector3));
		}

		for (int i = 0; i < mesh.m_Faces.size(); i++)
		{
			// Write Face
			OutStream.write(reinterpret_cast<const char*>(&mesh.m_Faces[i].m_NumIndices), sizeof(int));

			for (int index : mesh.m_Faces[i].m_Indices)
			{
				OutStream.write(reinterpret_cast<const char*>(&index), sizeof(int));
			}
		}
	}

	OutStream.close();

	return true;
}

bool MeshAsset::LoadAsset(const std::string& AssetName)
{
	PROFILE_FUNCTION();

	std::ifstream InputStream = std::ifstream(AssetName, std::ios::in | std::ios::binary);
	
	if (InputStream.is_open() == false)
	{
		FLOW_ENGINE_ERROR("Failed to open file %s", AssetName.c_str());
		return false;
	}
	
	size_t MeshCount;
	InputStream.read(reinterpret_cast<char*>(&MeshCount), sizeof(size_t));
	
	if (MeshCount == 0)
	{
		FLOW_ENGINE_LOG("File was empty");
		return false;
	}
	
	for(size_t it = 0; it < MeshCount; it++)
	{
		//Mesh* NewMesh = new Mesh(this, static_cast<int>(m_Meshes.size()));
		Mesh NewMesh = Mesh(this, static_cast<int>(m_Meshes.size()));
		size_t NVertices = 0;
		size_t NFaces = 0;
	
		// Collision Name

		//Make sure to resize the string so we dont overflow

		char NameBuffer[64] = "";
		InputStream.read(reinterpret_cast<char*>(&NameBuffer), sizeof(char) * 64);
		NewMesh.m_CollisionName = NameBuffer;
	
		// Number of Vertices
		InputStream.read(reinterpret_cast<char*>(&NVertices), sizeof(size_t));
	
		// Number of Faces
		InputStream.read(reinterpret_cast<char*>(&NFaces), sizeof(size_t));
	
		if (NVertices == 0 || NFaces == 0)
		{
			FLOW_ENGINE_ERROR("File had incorrect number of verts/faces");
			return false;
		}
	
		NewMesh.m_Vertices.reserve(NVertices);
		NewMesh.m_Normals.reserve(NVertices);
		NewMesh.m_TexCoords.reserve(NVertices);
		NewMesh.m_Faces.reserve(NFaces);
	
		for (size_t i = 0; i < NVertices; i++)
		{
			Vector3 Vertex;
			Vector3 Normal;
			Vector3 TexCoord;
	
			// Write Vertex position
			InputStream.read(reinterpret_cast<char*>(&Vertex), sizeof(Vector3));
	
			// Write Normal
			InputStream.read(reinterpret_cast<char*>(&Normal), sizeof(Vector3));
	
			// Write TexCoord
			InputStream.read(reinterpret_cast<char*>(&TexCoord), sizeof(Vector3));
	
			NewMesh.m_Vertices.push_back(Vertex);
			NewMesh.m_Normals.push_back(Normal);
			NewMesh.m_TexCoords.push_back(TexCoord);
		}
	
		for (size_t i = 0; i < NFaces; i++)
		{
			Mesh::Face NewFace(0, 0);
	
			InputStream.read(reinterpret_cast<char*>(&NewFace.m_NumIndices), sizeof(int));

			for (unsigned int j = 0; j < NewFace.m_NumIndices; j++)
			{
				int LoadedIndex;
				InputStream.read(reinterpret_cast<char*>(&LoadedIndex), sizeof(int));
				NewFace.m_Indices.push_back(LoadedIndex);
			}
	
			NewMesh.m_Faces.push_back(NewFace);
		}
	
		m_Meshes.push_back(NewMesh);
	}

	return true;


}

std::vector<Mesh> MeshAsset::GetAllMeshes() const
{
	return m_Meshes;
}

Mesh* MeshAsset::GetMesh(int Index)
{
	return (m_Meshes.size() > 0) ? &(m_Meshes[Index]) : nullptr;
}

void MeshAsset::GenerateAssetSize()
{
	m_AssetSize = 0;
	for (auto& Mesh : m_Meshes)
	{
		size_t VecSize = sizeof(Vector3);
		m_AssetSize +=
			(VecSize * Mesh.m_Vertices.size()) +
			(VecSize * Mesh.m_Normals.size()) +
			(VecSize * Mesh.m_TexCoords.size()) +
			(sizeof(Mesh::Face) * Mesh.m_Faces.size());
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

std::vector<Bindables::Bindable*> Mesh::GenerateBinds(VertexLayout& OutVertexLayout)
{
	//Dont regenerate if already done
	if (m_Binds.size() != 0)
	{
		OutVertexLayout = m_VertexLayout;
		return m_Binds;
	}

	m_Topology = static_cast<Bindables::Topology*>(Bindables::Topology::Resolve(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
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

	Array<uint16> indices;
	indices.Reserve((uint32)(GetNumFaces() * 3)); //Using triangles, change for quads
	for (const Mesh::Face& Face : GetFaces())
	{
		assert(Face.m_NumIndices == 3);
		indices.Add(static_cast<uint16>(Face.m_Indices[0]));
		indices.Add(static_cast<uint16>(Face.m_Indices[1]));
		indices.Add(static_cast<uint16>(Face.m_Indices[2]));
	}

	//Add Vertex Buffer Bind
	std::string name = (m_Parent->GetAssetName() + std::to_string(m_MeshIndex));
	HashString testHash(name.c_str());
	m_BindableVBuffer = static_cast<Bindables::VertexBuffer*>(Bindables::VertexBuffer::Resolve(testHash, VBuffer));
	m_Binds.push_back(m_BindableVBuffer);

	//Bind Index Buffer
	assert("MeshAsset::GenerateBinds: Cannot bind multiple index buffers." && m_IndexBuffer == nullptr);
	m_IndexBuffer = static_cast<Bindables::IndexBuffer*>(Bindables::IndexBuffer::Resolve(HashString((m_Parent->GetAssetName() + std::to_string(m_MeshIndex)).c_str()), indices));
	m_Binds.push_back(m_IndexBuffer);

	OutVertexLayout = m_VertexLayout;

	return m_Binds;
}

const Bindables::IndexBuffer* Mesh::GetIndexBuffer() const
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