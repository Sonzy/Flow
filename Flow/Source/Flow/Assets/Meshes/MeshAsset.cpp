#include "Flowpch.h"
#include "MeshAsset.h"


#include "Flow\Application.h"

#include <Assimp/Importer.hpp>
#include <Assimp/scene.h>
#include <Assimp/postprocess.h>

namespace Flow
{
	bool MeshAsset::LoadAsset(const std::string& LocalPath)
	{
		Assimp::Importer Importer;
		const aiScene* Model = Importer.ReadFile(Application::GetApplication().GetLocalFilePath() + LocalPath,
			aiProcess_Triangulate |
			aiProcess_JoinIdenticalVertices |
			aiProcess_ConvertToLeftHanded);

		if (!Model)
			return false;

		//Load the vertices
		const aiMesh* Mesh = Model->mMeshes[0];
		for (unsigned int i = 0; i < Mesh->mNumVertices; i++)
		{
			m_Vertices.emplace_back(
				Vector(Mesh->mVertices[i].x, Mesh->mVertices[i].y, Mesh->mVertices[i].z),
				Vector(Mesh->mTextureCoords[0][i].x, Mesh->mTextureCoords[0][i].y, Mesh->mTextureCoords[0][i].z)
				);
		}


		//Load the faces
		m_Faces.reserve(Mesh->mNumFaces);
		for (unsigned int i = 0; i < Mesh->mNumFaces; i++)
		{
			//TODO: Check that the mesh is actually triangles instead of quads
			const auto& face = Mesh->mFaces[i];
			m_Faces.emplace_back(face.mNumIndices, face.mIndices);
		}

		FLOW_ENGINE_LOG("MeshAsset::LoadAsset: Successfully loaded static mesh {0}", LocalPath);

		return true;
	}

}

