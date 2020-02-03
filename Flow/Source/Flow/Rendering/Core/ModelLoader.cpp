#include "Flowpch.h"
#include "ModelLoader.h"

namespace Flow
{
	ModelLoader* ModelLoader::s_ModelLoader = new ModelLoader();

	Flow::ModelLoader::ModelLoader()
	{
	}

	bool Flow::ModelLoader::LoadModel(const std::string Path)
	{
		s_ModelLoader->Vertices.clear();

		Assimp::Importer Importer;
		const aiScene* Model = Importer.ReadFile(Path,
			aiProcess_Triangulate |
			aiProcess_JoinIdenticalVertices);

		if (!Model)
		{
			FLOW_ENGINE_ERROR("{0}", Importer.GetErrorString());
			return false;
		}

		//Load the data from each vertex
		const auto Mesh = Model->mMeshes[0];
		for (unsigned int i = 0; i < Mesh->mNumVertices; i++)
		{
			s_ModelLoader->Vertices.emplace_back(
				DirectX::XMFLOAT3{ Mesh->mVertices[i].x, Mesh->mVertices[i].y, Mesh->mVertices[i].z }
			);
		}

		return true;
	}

	std::vector<DirectX::XMFLOAT3> Flow::ModelLoader::GetVerts()
	{
		return s_ModelLoader->Vertices;
	}

	//int ModelLoader::GetFaces()
	//{
	//	return s_ModelLoader->m_NumFaces;
	//}

}