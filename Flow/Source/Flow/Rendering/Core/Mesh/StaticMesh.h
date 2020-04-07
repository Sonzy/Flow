#pragma once
#include "Flow/Core.h"
#include "Flow\Assets\Meshes\MeshAsset.h"
#include "Flow/Rendering/Core/RenderableBase.h"
#include <DirectXMath.h>

namespace Flow
{
	class Material;

	class FLOW_API StaticMesh : public RenderableBase
	{
	public:

		StaticMesh(const std::string& LocalPath);

		void InitialiseStaticMesh(const std::string& LocalPath, Material* MaterialOverride);

		virtual DirectX::XMMATRIX GetTransformXM() const override;
	};
}