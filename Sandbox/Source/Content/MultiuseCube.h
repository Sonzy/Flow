#pragma once
#include "Flow/GameFramework/WorldObject.h"

namespace Flow
{
	class StaticMeshComponent;
}

class MultiuseCube : public Flow::WorldObject
{
public:
	MultiuseCube();

private:

	std::shared_ptr<Flow::StaticMeshComponent> m_StaticMesh;
};