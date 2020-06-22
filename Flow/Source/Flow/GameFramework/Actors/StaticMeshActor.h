#pragma once
#include "Flow/GameFramework/Actor.h"

class StaticMeshComponent;

class FLOW_API StaticMeshActor : public Actor
{
public:
	StaticMeshActor();
	StaticMeshActor(const std::string& NewName);
	virtual ~StaticMeshActor();

	StaticMeshComponent* GetMeshComponent() const;

protected:

	std::shared_ptr<StaticMeshComponent> _MeshComponent;
};