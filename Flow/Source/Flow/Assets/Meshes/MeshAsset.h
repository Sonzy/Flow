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
	public:
		MeshVertex(Vector VertexPos, Vector TexCoord)
			: VertexPosition(VertexPos), TextureCoord(TexCoord)
		{}
		
		Vector VertexPosition;
		Vector TextureCoord;
	};


	class MeshAsset : public AssetBase
	{
	public:

		MeshAsset()
			: m_AssetSize(0)
		{};

		virtual bool LoadAsset(const std::string& LocalPath) override;

		const std::vector<MeshVertex>& GetVertices() {	return m_Vertices;	};
		const std::vector<MeshFace>& GetFaces() { return m_Faces; };

		const size_t GetNumFaces() { return m_Faces.size(); };

	protected:


		std::vector<MeshVertex> m_Vertices;
		std::vector<MeshFace> m_Faces;
		size_t m_AssetSize;
	};
}