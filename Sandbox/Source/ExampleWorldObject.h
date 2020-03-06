#pragma once
#include "Flow\GameFramework\WorldObject.h"
#include "Flow\GameFramework\Components\StaticMeshComponent.h"

class ExampleWorldObject : public Flow::WorldObject
{
public:
	ExampleWorldObject();
	ExampleWorldObject(const std::string& NewName);
	~ExampleWorldObject() = default;

protected:

	std::shared_ptr<Flow::StaticMeshComponent> TestMesh;
};