#include "Flowpch.h"
#include "MeshAsset.h"
#include "Flow\Application.h"

#include <Assimp/Importer.hpp>
#include <Assimp/scene.h>
#include <Assimp/postprocess.h>

#include "Flow\Assets\AssetSystem.h"

#include "Flow\Rendering\Core\Bindables\Topology.h"
#include "Flow\Rendering\Core\Bindables\BindableVertexBuffer.h"

namespace Flow
{
	MeshAsset::MeshAsset()
	{
	}

	MeshAsset::~MeshAsset()
	{
		Meshes_.clear();
	}

	bool MeshAsset::LoadAsset(const std::string& LocalPath)
	{
		Assimp::Importer Importer;
		const aiScene* Scene = Importer.ReadFile(Application::GetApplication().GetLocalFilePath() + LocalPath,
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
				Mesh* NewMesh = new Mesh(this, Meshes_.size());

				//Load the vertices
				const aiMesh* AIMesh = Scene->mMeshes[Scene->mRootNode->mChildren[i]->mMeshes[0]];
				for (unsigned int i = 0; i < AIMesh->mNumVertices; i++)
				{
					NewMesh->Vertices_.emplace_back(Vector(AIMesh->mVertices[i].x, AIMesh->mVertices[i].y, AIMesh->mVertices[i].z));
					NewMesh->Normals_.emplace_back(Vector(AIMesh->mNormals[i].x, AIMesh->mNormals[i].y, AIMesh->mNormals[i].z));
					NewMesh->TexCoords_.emplace_back(Vector(AIMesh->mTextureCoords[0][i].x, AIMesh->mTextureCoords[0][i].y, AIMesh->mTextureCoords[0][i].z));
				}

				//Load the faces
				NewMesh->Faces_.reserve(AIMesh->mNumFaces);
				for (unsigned int i = 0; i < AIMesh->mNumFaces; i++)
				{
					const auto& face = AIMesh->mFaces[i];
					NewMesh->Faces_.emplace_back(face.mNumIndices, face.mIndices);
				}

				Meshes_.push_back(NewMesh);
			}
		}

		GenerateAssetSize();
		AssetType_ = EAssetType::Mesh;

		return true;
	}

	std::vector<Mesh*> MeshAsset::GetAllMeshes() const
	{
		return Meshes_;
	}

	Mesh* MeshAsset::GetMesh(int Index) const
	{
		return Meshes_[Index];
	}

	void MeshAsset::GenerateAssetSize()
	{
		AssetSize_ = 0;
		for (auto& Mesh : Meshes_)
		{
			size_t VecSize = sizeof(Vector);
			AssetSize_ +=
				(VecSize * Mesh->Vertices_.size()) +
				(VecSize * Mesh->Normals_.size()) +
				(VecSize * Mesh->TexCoords_.size()) +
				(sizeof(MeshFace) * Mesh->Faces_.size());
		}
	}

	//= Mesh ======================

	Mesh::Mesh(MeshAsset* Parent, int MeshIndex)
		: Parent_(Parent), MeshIndex_(MeshIndex), CollisionName_("")
	{
	}

	Mesh::~Mesh()
	{
		Vertices_.clear();
		Normals_.clear();
		TexCoords_.clear();
		Faces_.clear();
		Binds_.clear();
	}

	const std::vector<Vector>& Mesh::GetCollisionVertices() const
	{
		return !CollisionName_.empty() ? AssetSystem::GetAsset<MeshAsset>(CollisionName_)->GetMesh(0)->Vertices_ : Vertices_;
	}

	std::vector<MeshVertex> Mesh::GetVertices() const
	{
		CHECK_ASSERT(Vertices_.size() == Normals_.size() && Normals_.size() == TexCoords_.size(),
			"MeshAsset::GetVertices: Property arrays are mismatched");

		std::vector<MeshVertex> Vertices;
		for (int i = 0; i < Vertices_.size(); i++)
		{
			Vertices.emplace_back(Vertices_[i], Normals_[i], TexCoords_[i]);
		}

		return Vertices;
	}

	std::vector<std::shared_ptr<Bindable>> Mesh::GenerateBinds(VertexLayout& OutVertexLayout)
	{
		//Dont regenerate if already done
		if (Binds_.size() != 0)
		{
			OutVertexLayout = VertexLayout_;
			return Binds_;
		}

		Binds_.push_back(Topology::Resolve(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

		// Define Vertex Layout
		VertexLayout_.Append(ElementType::Position3D);
		VertexLayout_.Append(ElementType::Normal);
		VertexLayout_.Append(ElementType::Texture2D);

		// Define Vertex Buffer information
		VertexBuffer VBuffer(VertexLayout_);

		for (auto& Vertex : GetVertices())
		{
			VBuffer.EmplaceBack( //TODO: Dont actually need to know whats in here, just need to know the stride and offsets
				DirectX::XMFLOAT3{ Vertex.Position_.X ,  Vertex.Position_.Y,  Vertex.Position_.Z },
				DirectX::XMFLOAT3{ Vertex.Normal_.X ,  Vertex.Normal_.Y,  Vertex.Normal_.Z },
				DirectX::XMFLOAT2{ Vertex.TexCoord_.X,  Vertex.TexCoord_.Y }
			);
		}

		std::vector<unsigned short> indices;
		indices.reserve(GetNumFaces() * 3); //Using triangles, change for quads
		for (auto& Face : GetFaces())
		{
			assert(Face.NumIndices_ == 3);
			indices.push_back(Face.Indices_[0]);
			indices.push_back(Face.Indices_[1]);
			indices.push_back(Face.Indices_[2]);
		}

		//Add Vertex Buffer Bind
		Binds_.push_back(BindableVertexBuffer::Resolve(Parent_->GetAssetName() + std::to_string(MeshIndex_), VBuffer));

		//Bind Index Buffer
		std::shared_ptr<Bindable> Index = IndexBuffer::Resolve(Parent_->GetAssetName() + std::to_string(MeshIndex_), indices);
		assert("MeshAsset::GenerateBinds: Cannot bind multiple index buffers." && IndexBuffer_ == nullptr);
		Binds_.push_back(Index);
		IndexBuffer_ = static_cast<IndexBuffer*>(Index.get());

		OutVertexLayout = VertexLayout_;

		return Binds_;
	}

	const IndexBuffer* Mesh::GetIndexBuffer() const
	{
		return IndexBuffer_;
	}
}