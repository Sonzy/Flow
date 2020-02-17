#pragma once
#include "Flow\Assets\AssetBase.h"
#include <string>
#include "Flow\Helper\Maths.h"
//class aiScene;

namespace Flow
{
	struct MeshFace
	{
		unsigned int m_NumIndices;
		std::vector<int> m_Indices;

		MeshFace(int NumIndices, unsigned int* Indices)
			: m_NumIndices(NumIndices)
		{
			m_Indices.reserve(m_NumIndices);
			for (int i = 0; i < m_NumIndices; i++)
			{
				m_Indices.push_back(Indices[i]);
			}
		}
	};

	struct MeshVertex
	{
		MeshVertex(Vector Pos, Vector Norm, Vector Tex)
			: Position(Pos), Normal(Norm), TexCoord(Tex)
		{}

		Vector Position;
		Vector Normal;
		Vector TexCoord;
	};

	class MeshAsset : public AssetBase
	{
	public:

		MeshAsset()
		{};

		virtual bool LoadAsset(const std::string& LocalPath) override;

		const std::vector<MeshFace>& GetFaces() { return m_Faces; };
		const size_t GetNumFaces() { return m_Faces.size(); };

		std::vector<MeshVertex> GetVertices() const;

	protected:

		void GenerateAssetSize();

		std::vector<Vector> m_Vertices;
		std::vector<Vector> m_Normals;
		std::vector<Vector> m_TexCoords;
		std::vector<MeshFace> m_Faces;
	};
}