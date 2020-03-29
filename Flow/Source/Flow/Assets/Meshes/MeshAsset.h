#pragma once
#include "Flow\Assets\AssetBase.h"
#include <string>
#include "Flow\Helper\Maths.h"


#include "Flow\Rendering\Core\Bindable.h"
#include "Flow\Rendering\Core\Vertex\VertexLayout.h"
#include "Flow\Rendering\Core\Renderable.h"

namespace Flow
{
	class IndexBuffer;
	class VertexLayout;
	class Material;

	struct MeshFace
	{
		unsigned int m_NumIndices;
		std::vector<int> m_Indices;

		MeshFace(int NumIndices, unsigned int* Indices)
			: m_NumIndices(NumIndices)
		{
			m_Indices.reserve(m_NumIndices);
			for (unsigned int i = 0; i < m_NumIndices; i++)
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

	//class FLOW_API MeshAsset : public AssetBase
	//{
	//public:
	//
	//	MeshAsset()	{};
	//	~MeshAsset();
	//
	//	virtual bool LoadAsset(const std::string& LocalPath) override;
	//
	//	const std::vector<MeshFace>& GetFaces() { return m_Faces; };
	//	const size_t GetNumFaces() { return m_Faces.size(); };
	//
	//	std::vector<MeshVertex> GetVertices() const;
	//
	//	//===============================
	//
	//	void SetMaterial(Material* NewMaterial);
	//	std::vector<std::shared_ptr<Bindable>> GenerateBinds(VertexLayout& OutVertexLayout);
	//	const IndexBuffer* GetIndexBuffer() const;
	//
	//protected:
	//
	//	void GenerateAssetSize();
	//
	//	std::vector<Vector> m_Vertices;
	//	std::vector<Vector> m_Normals;
	//	std::vector<Vector> m_TexCoords;
	//	std::vector<MeshFace> m_Faces;
	//
	//	const IndexBuffer* m_IndexBuffer;
	//	Material* m_Material;
	//	VertexLayout m_VertexLayout;
	//	std::vector<std::shared_ptr<Bindable>> m_Binds;
	//};

	class IndexBuffer;
	class Material;
	class MeshAsset;

	class Mesh
	{
	public:
		Mesh(MeshAsset* Parent, int MeshIndex);

		std::vector<Vector> m_Vertices;
		std::vector<Vector> m_Normals;
		std::vector<Vector> m_TexCoords;
		std::vector<MeshFace> m_Faces;

		// Optional variable, allows for a mesh to be specified to be used for collision
		std::string CollisionName_;

		Material* m_Material;
		VertexLayout m_VertexLayout;

		const IndexBuffer* m_IndexBuffer;
		std::vector<std::shared_ptr<Bindable>> m_Binds;

		[[nodiscard]] const std::vector<Vector>& GetCollisionVertices() const;

	public:

		const std::vector<MeshFace>& GetFaces() { return m_Faces; };
		const size_t GetNumFaces() { return m_Faces.size(); };

		std::vector<MeshVertex> GetVertices() const;

		void SetMaterial(Material* NewMaterial);
		std::vector<std::shared_ptr<Bindable>> GenerateBinds(VertexLayout& OutVertexLayout);
		const IndexBuffer* GetIndexBuffer() const;

		MeshAsset* m_Parent;
		int m_MeshIndex;
	};

	class FLOW_API MeshAsset : public AssetBase
	{
	public:
		MeshAsset();
		~MeshAsset();

		virtual bool LoadAsset(const std::string& LocalPath) override;

		Mesh* GetMesh(int Index) const;
		std::vector<Mesh*> GetAllMeshes() const;

	private:
		void GenerateAssetSize();

		std::vector<Mesh*> m_Meshes;
	};
}