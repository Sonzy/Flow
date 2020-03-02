#pragma once
#include "Flow/GameFramework/WorldObject.h"

namespace Flow
{
	class StaticMeshComponent;
}

class OpenCVTest : public Flow::WorldObject
{
public:
	OpenCVTest();

private:

	std::shared_ptr<Flow::StaticMeshComponent> m_StaticMesh;
};