#pragma once
#include "Flow/Core.h"
#include "Flow/Rendering/Core/Renderable.h"

namespace Flow
{
	class FLOW_API StaticMesh : public Renderable
	{
	public:

		StaticMesh(const std::string& LocalPath);
	};
}