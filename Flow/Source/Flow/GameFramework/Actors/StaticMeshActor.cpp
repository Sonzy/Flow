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
	m_MeshComponent = CreateComponent<StaticMeshComponent>("Mesh Component");
	m_RootComponent = m_MeshComponent;
}

StaticMeshActor::~StaticMeshActor()
{
	FLOW_ENGINE_LOG("StaticMeshActor::~StaticMeshActor");

	delete m_MeshComponent;
}

StaticMeshComponent* StaticMeshActor::GetMeshComponent() const
{
	return m_MeshComponent;
}
