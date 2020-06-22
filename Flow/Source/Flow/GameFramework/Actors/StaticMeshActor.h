#pragma once
#include "Flow/GameFramework/Actor.h"

class StaticMeshComponent;

class StaticMeshActor : public Actor
{
public:
	StaticMeshActor();
	StaticMeshActor(const std::string& NewName);

	StaticMeshComponent* GetMeshComponent() const;

protected:

	std::shared_ptr<StaticMeshComponent> _MeshComponent;
};