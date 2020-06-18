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
	_Meshes.clear();
}

bool MeshAsset::LoadAsset(const std::string& LocalPath)
{
	Assimp::Importer Importer;
	const aiScene* Scene = Importer.ReadFile(LocalPath,
		aiProcess_Triangulate |  //Force Tris
		aiProcess_JoinIdenticalVertices |
		aiProcess_ConvertToLeftHanded);

	CHECK_RETURN_FALSE(!Scene, "MeshAsset::LoadAsset: Failed to load model from path ({0})", LocalPath);

	//TODO: FBX doesnt load right
	aiNode* Root = Scene->mRootNode;
	for (int i = 0; i < Scene->mRootNode->mNumChildren; i++)
	{
		for (int j = 0; j < Scene->mRootNode->mChildren[i]->mNumMeshes; j++)
		{
			Mesh* NewMesh = new Mesh(this, _Meshes.size());

			//Load the vertices
			const aiMesh* AIMesh = Scene->mMeshes[Scene->mRootNode->mChildren[i]->mMeshes[0]];
			for (unsigned int i = 0; i < AIMesh->mNumVertices; i++)
			{
				NewMesh->_Vertices.emplace_back(Vector(AIMesh->mVertices[i].x, AIMesh->mVertices[i].y, AIMesh->mVertices[i].z));
				NewMesh->_Normals.emplace_back(Vector(AIMesh->mNormals[i].x, AIMesh->mNormals[i].y, AIMesh->mNormals[i].z));
				NewMesh->_TexCoords.emplace_back(Vector(AIMesh->mTextureCoords[0][i].x, AIMesh->mTextureCoords[0][i].y, AIMesh->mTextureCoords[0][i].z));
			}

			//Load the faces
			NewMesh->_Faces.reserve(AIMesh->mNumFaces);
			for (unsigned int i = 0; i < AIMesh->mNumFaces; i++)
			{
				const auto& face = AIMesh->mFaces[i];
				NewMesh->_Faces.emplace_back(face.mNumIndices, face.mIndices);
			}

			_Meshes.push_back(NewMesh);
		}
	}

	GenerateAssetSize();
	_AssetType = EAssetType::Mesh;

	return true;
}

std::vector<Mesh*> MeshAsset::GetAllMeshes() const
{
	return _Meshes;
}

Mesh* MeshAsset::GetMesh(int Index) const
{
	return (_Meshes.size() > 0) ? _Meshes[Index] : nullptr;
}

void MeshAsset::GenerateAssetSize()
{
	_AssetSize = 0;
	for (auto& Mesh : _Meshes)
	{
		size_t VecSize = sizeof(Vector);
		_AssetSize +=
			(VecSize * Mesh->_Vertices.size()) +
			(VecSize * Mesh->_Normals.size()) +
			(VecSize * Mesh->_TexCoords.size()) +
			(sizeof(MeshFace) * Mesh->_Faces.size());
	}
}

//= Mesh ======================

Mesh::Mesh(MeshAsset* Parent, int MeshIndex)
	: _Parent(Parent), _MeshIndex(MeshIndex), _CollisionName("")
{
}

Mesh::~Mesh()
{
	_Vertices.clear();
	_Normals.clear();
	_TexCoords.clear();
	_Faces.clear();
	_Binds.clear();
}

const std::vector<Vector>& Mesh::GetCollisionVertices() const
{
	return !_CollisionName.empty() ? AssetSystem::GetAsset<MeshAsset>(_CollisionName)->GetMesh(0)->_Vertices : _Vertices;
}

std::vector<MeshVertex> Mesh::GetVertices() const
{
	CHECK_ASSERT(_Vertices.size() == _Normals.size() && _Normals.size() == _TexCoords.size(),
		"MeshAsset::GetVertices: Property arrays are mismatched");

	std::vector<MeshVertex> Vertices;
	for (int i = 0; i < _Vertices.size(); i++)
	{
		Vertices.emplace_back(_Vertices[i], _Normals[i], _TexCoords[i]);
	}

	return Vertices;
}

std::vector<std::shared_ptr<Bindable>> Mesh::GenerateBinds(VertexLayout& OutVertexLayout)
{
	//Dont regenerate if already done
	if (_Binds.size() != 0)
	{
		OutVertexLayout = _VertexLayout;
		return _Binds;
	}

	_Topology = std::dynamic_pointer_cast<Topology>(Topology::Resolve(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
	_Binds.push_back(_Topology);

	// Define Vertex Layout
	_VertexLayout.Append(ElementType::Position3D);
	_VertexLayout.Append(ElementType::Normal);
	_VertexLayout.Append(ElementType::Texture2D);

	// Define Vertex Buffer information
	VertexBuffer VBuffer(_VertexLayout);

	for (auto& Vertex : GetVertices())
	{
		VBuffer.EmplaceBack( //TODO: Dont actually need to know whats in here, just need to know the stride and offsets
			DirectX::XMFLOAT3{ Vertex._Position.X ,  Vertex._Position.Y,  Vertex._Position.Z },
			DirectX::XMFLOAT3{ Vertex._Normal.X ,  Vertex._Normal.Y,  Vertex._Normal.Z },
			DirectX::XMFLOAT2{ Vertex._TexCoord.X,  Vertex._TexCoord.Y }
		);
	}

	std::vector<unsigned short> indices;
	indices.reserve(GetNumFaces() * 3); //Using triangles, change for quads
	for (auto& Face : GetFaces())
	{
		assert(Face._NumIndices == 3);
		indices.push_back(Face._Indices[0]);
		indices.push_back(Face._Indices[1]);
		indices.push_back(Face._Indices[2]);
	}

	//Add Vertex Buffer Bind
	_BindableVBuffer = std::dynamic_pointer_cast<BindableVertexBuffer>(BindableVertexBuffer::Resolve(_Parent->GetAssetName() + std::to_string(_MeshIndex), VBuffer));
	_Binds.push_back(_BindableVBuffer);

	//Bind Index Buffer
	assert("MeshAsset::GenerateBinds: Cannot bind multiple index buffers." && _IndexBuffer == nullptr);
	_IndexBuffer = std::dynamic_pointer_cast<IndexBuffer>(IndexBuffer::Resolve(_Parent->GetAssetName() + std::to_string(_MeshIndex), indices));
	_Binds.push_back(_IndexBuffer);

	OutVertexLayout = _VertexLayout;

	return _Binds;
}

std::shared_ptr<IndexBuffer> Mesh::GetIndexBuffer() const
{
	return _IndexBuffer;
}