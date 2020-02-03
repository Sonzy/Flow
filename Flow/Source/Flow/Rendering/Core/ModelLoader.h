#pragma once
#include <string>
#include <DirectXMath.h>
#include <vector>

#include <Assimp/Importer.hpp>
#include <Assimp/scene.h>
#include <Assimp/postprocess.h>

namespace Flow
{
	class ModelLoader
	{
	public:

		ModelLoader();

		static bool LoadModel(const std::string Path);

		static std::vector<DirectX::XMFLOAT3> GetVerts();

	private:

		std::vector<DirectX::XMFLOAT3> Vertices;
		int m_NumFaces;
		//std::vector<

		static ModelLoader* s_ModelLoader;
	};
}