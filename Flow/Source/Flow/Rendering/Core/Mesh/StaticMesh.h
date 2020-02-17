#pragma once
#include "Flow/Core.h"
#include "Flow\Assets\Meshes\MeshAsset.h"
#include "Flow/Rendering/Core/RenderableBase.h"

class aiMesh;


namespace Flow
{
	class Material;

	class FLOW_API StaticMesh : public RenderableBase
	{
	public:

		StaticMesh(const std::string& LocalPath);

		virtual DirectX::XMMATRIX GetTransformXM() const override;

		MeshAsset* m_Mesh;
		Material* m_Material;
	};
}