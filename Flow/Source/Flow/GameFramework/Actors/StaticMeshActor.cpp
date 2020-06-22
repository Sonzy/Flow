#include "Flowpch.h"
#include "StaticMeshActor.h"
#include "Flow/GameFramework/Components/StaticMeshComponent.h"

StaticMeshActor::StaticMeshActor()
	: StaticMeshActor("Static Mesh Actor")
{
}

StaticMeshActor::StaticMeshActor(const std::string& NewName)
	: Actor(NewName)
{
	_MeshComponent = CreateComponent<StaticMeshComponent>("Mesh Component");
	_RootComponent = _MeshComponent.get();
}

StaticMeshComponent* StaticMeshActor::GetMeshComponent() const
{
	return _MeshComponent.get();
}
