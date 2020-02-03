#pragma once
#include "Flow/Core.h"
#include "Flow/Rendering/Core/RenderableBase.h"

class aiMesh;

namespace Flow
{
	class FLOW_API StaticMesh : public RenderableBase
	{
	public:

		StaticMesh(const std::string& LocalPath);

		virtual DirectX::XMMATRIX GetTransformXM() const override;
	};
}