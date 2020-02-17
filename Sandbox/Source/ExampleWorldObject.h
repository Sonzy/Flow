#pragma once
#include "Flow\GameFramework\WorldObject.h"
#include "Flow\GameFramework\Components\StaticMeshComponent.h"

class ExampleWorldObject : public Flow::WorldObject
{
public:
	ExampleWorldObject();

protected:

	std::shared_ptr<Flow::StaticMeshComponent> TestMesh;
};