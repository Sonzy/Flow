#pragma once

//= Includes ==============================
#include <string>

#include "Maths/Vector3.h"

#include "Rendering\Core\Bindable.h"
#include "Rendering\Core\Vertex\VertexLayout.h"
#include "Rendering\Core\Renderable.h"

#include "Assets\Asset.h"

//= Forward Declarations ==================
class IndexBuffer;
class VertexLayout;
class Material;
class MeshAsset;

/* Class for a single mesh. A mesh asset can contain multiple of these */
class Mesh
{
public:

	//= Public Struct Definitions ====================

	/* Stores the properties for each mesh face. Includes indices */
	struct Face
	{
							Face(int NumIndices, unsigned int* Indices);
							~Face();

		unsigned int		m_NumIndices;
		std::vector<int>	m_Indices;
	};

	/* Stores all properties of a vertex */
	struct Vertex
	{
		Vertex(Vector3 Pos, Vector3 Norm, Vector3 Tex)
			: m_Position(Pos), m_Normal(Norm), m_TexCoord(Tex)
		{}

		Vector3 m_Position;
		Vector3 m_Normal;
		Vector3 m_TexCoord;
	};

public:

	//= Public Functions =================================

												Mesh(MeshAsset* Parent, int MeshIndex);
												~Mesh();

	/* Returns a collision mesh if specified, otherwise returns the current mesh. */
	const std::vector<Vector3>&					GetCollisionVertices() const;

	//= Public Variables =================================

	// Optional variable, allows for a mesh to be specified to be used for collision
	std::string									m_CollisionName;

	//= MeshValues =

	std::vector<Vector3>							m_Vertices;
	std::vector<Vector3>							m_Normals;
	std::vector<Vector3>							m_TexCoords;
	std::vector<Mesh::Face>						m_Faces;

	//const IndexBuffer* _IndexBuffer;
	std::vector<std::shared_ptr<Bindable>>		m_Binds;

	//TODO: Clean up, these are the necessary binds for the mesh specifically
	std::shared_ptr<BindableVertexBuffer>		m_BindableVBuffer;
	std::shared_ptr<IndexBuffer>				m_IndexBuffer;
	std::shared_ptr<Topology>					m_Topology;
	VertexLayout								m_VertexLayout;

public:

	const std::vector<Mesh::Face>&				GetFaces() const { return m_Faces; };
	const size_t								GetNumFaces() const { return m_Faces.size(); };
	std::vector<Mesh::Vertex>					GetVertices() const;
	std::shared_ptr<IndexBuffer>				GetIndexBuffer() const;

	/* Creates and returns a vector of all binds required from this mesh */
	std::vector<std::shared_ptr<Bindable>>		GenerateBinds(VertexLayout& OutVertexLayout);

	// Asset this mesh belongs to
	MeshAsset*									m_Parent;
	// The index of this mesh in the asset
	int											m_MeshIndex;
};

/* Asset class containing multiple meshes, used for static and skeletal meshes */
class FLOW_API MeshAsset : public Asset
{
public:
												MeshAsset();
	virtual										~MeshAsset();

	virtual bool								LoadAsset(const std::string& LocalPath) override;

	Mesh*										GetMesh(int Index) const;
	std::vector<Mesh*>							GetAllMeshes() const;

private:
	void										GenerateAssetSize();

	std::vector<Mesh*>							m_Meshes;
};