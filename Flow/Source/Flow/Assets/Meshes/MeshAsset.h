#pragma once
#include "Flow\Assets\AssetBase.h"
#include <string>
#include "Flow\Helper\Maths.h"


#include "Flow\Rendering\Core\Bindable.h"
#include "Flow\Rendering\Core\Vertex\VertexLayout.h"
#include "Flow\Rendering\Core\Renderable.h"

class IndexBuffer;
class VertexLayout;
class Material;
class MeshAsset;

/* Stores the properties for each mesh face. Includes indices */
struct MeshFace
{
	unsigned int _NumIndices;
	std::vector<int> _Indices;

	MeshFace(int NumIndices, unsigned int* Indices)
		: _NumIndices(NumIndices)
	{
		_Indices.reserve(_NumIndices);
		for (unsigned int i = 0; i < _NumIndices; i++)
		{
			_Indices.push_back(Indices[i]);
		}
	}

	~MeshFace()
	{
		_Indices.clear();
	}
};

/* Stores all properties of a vertex */
struct MeshVertex
{
	MeshVertex(Vector Pos, Vector Norm, Vector Tex)
		: _Position(Pos), _Normal(Norm), _TexCoord(Tex)
	{}

	Vector _Position;
	Vector _Normal;
	Vector _TexCoord;
};

/* Class for a single mesh. A mesh asset can contain multiple of these */
class Mesh
{
public:
	Mesh(MeshAsset* Parent, int MeshIndex);
	~Mesh();

	/* Returns a collision mesh if specified, otherwise returns the current mesh. */
	[[nodiscard]] const std::vector<Vector>& GetCollisionVertices() const;
	// Optional variable, allows for a mesh to be specified to be used for collision
	std::string _CollisionName;

	//= MeshValues =

	std::vector<Vector> _Vertices;
	std::vector<Vector> _Normals;
	std::vector<Vector> _TexCoords;
	std::vector<MeshFace> _Faces;

	//const IndexBuffer* _IndexBuffer;
	std::vector<std::shared_ptr<Bindable>> _Binds;

	//TODO: Clean up, these are the necessary binds for the mesh specifically
	std::shared_ptr<BindableVertexBuffer> _BindableVBuffer;
	std::shared_ptr<IndexBuffer> _IndexBuffer;
	std::shared_ptr<Topology> _Topology;
	VertexLayout _VertexLayout;

public:

	[[nodiscard]] const std::vector<MeshFace>& GetFaces() const { return _Faces; };
	[[nodiscard]] const size_t GetNumFaces() const { return _Faces.size(); };
	[[nodiscard]] std::vector<MeshVertex> GetVertices() const;
	[[nodiscard]] std::shared_ptr<IndexBuffer> GetIndexBuffer() const;

	/* Creates and returns a vector of all binds required from this mesh */
	std::vector<std::shared_ptr<Bindable>> GenerateBinds(VertexLayout& OutVertexLayout);

	// Asset this mesh belongs to
	MeshAsset* _Parent;
	// The index of this mesh in the asset
	int _MeshIndex;
};

/* Asset class containing multiple meshes, used for static and skeletal meshes */
class FLOW_API MeshAsset : public AssetBase
{
public:
	MeshAsset();
	virtual ~MeshAsset();

	virtual bool LoadAsset(const std::string& LocalPath) override;

	[[nodiscard]] Mesh* GetMesh(int Index) const;
	[[nodiscard]] std::vector<Mesh*> GetAllMeshes() const;

private:
	void GenerateAssetSize();

	std::vector<Mesh*> _Meshes;
};