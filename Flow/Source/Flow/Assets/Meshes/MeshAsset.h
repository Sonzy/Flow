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
	class MeshAsset;

	/* Stores the properties for each mesh face. Includes indices */
	struct MeshFace
	{
		unsigned int NumIndices_;
		std::vector<int> Indices_;

		MeshFace(int NumIndices, unsigned int* Indices)
			: NumIndices_(NumIndices)
		{
			Indices_.reserve(NumIndices_);
			for (unsigned int i = 0; i < NumIndices_; i++)
			{
				Indices_.push_back(Indices[i]);
			}
		}

		~MeshFace()
		{
			Indices_.clear();
		}
	};

	/* Stores all properties of a vertex */
	struct MeshVertex
	{
		MeshVertex(Vector Pos, Vector Norm, Vector Tex)
			: Position_(Pos), Normal_(Norm), TexCoord_(Tex)
		{}

		Vector Position_;
		Vector Normal_;
		Vector TexCoord_;
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
		std::string CollisionName_;

		//= MeshValues =

		std::vector<Vector> Vertices_;
		std::vector<Vector> Normals_;
		std::vector<Vector> TexCoords_;
		std::vector<MeshFace> Faces_;

		const IndexBuffer* IndexBuffer_;
		std::vector<std::shared_ptr<Bindable>> Binds_;
		VertexLayout VertexLayout_;

	public:

		[[nodiscard]] const std::vector<MeshFace>& GetFaces() const { return Faces_; };
		[[nodiscard]] const size_t GetNumFaces() const { return Faces_.size(); };
		[[nodiscard]] std::vector<MeshVertex> GetVertices() const;
		[[nodiscard]] const IndexBuffer* GetIndexBuffer() const;

		/* Creates and returns a vector of all binds required from this mesh */
		std::vector<std::shared_ptr<Bindable>> GenerateBinds(VertexLayout& OutVertexLayout);

		// Asset this mesh belongs to
		MeshAsset* Parent_;
		// The index of this mesh in the asset
		int MeshIndex_;
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

		std::vector<Mesh*> Meshes_;
	};
}