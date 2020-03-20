#pragma once
#include "Flow\GameFramework\WorldObject.h"
#include "Flow\GameFramework\Components\StaticMeshComponent.h"

class ExampleWorldObject : public Flow::WorldObject
{
public:
	ExampleWorldObject();
	ExampleWorldObject(const std::string& NewName);
	~ExampleWorldObject() = default;

	virtual void Tick(float DeltaTime) override;

	virtual void InitialisePhysics() override;

protected:

	std::shared_ptr<Flow::StaticMeshComponent> TestMesh;
	std::shared_ptr<Flow::StaticMeshComponent> TestMesh2;
};