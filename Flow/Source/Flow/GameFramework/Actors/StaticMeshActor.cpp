#include "pch.h"
#include "StaticMeshActor.h"
#include "Flow/GameFramework/Components/StaticMeshComponent.h"
#include "Assets/Meshes/MeshAsset.h"

#include <yaml-cpp/yaml.h>

StaticMeshActor::StaticMeshActor()
	: StaticMeshActor("Static Mesh Actor")
{
}

StaticMeshActor::StaticMeshActor(const std::string& NewName)
	: Actor(NewName)
{
	m_MeshComponent = CreateComponent<StaticMeshComponent>("Mesh Component");
	m_rootComponent = m_MeshComponent;
	m_MeshComponent->SetOwned(false);
}

StaticMeshActor::~StaticMeshActor()
{

}

StaticMeshComponent* StaticMeshActor::GetMeshComponent() const
{
	return m_MeshComponent;
}

void StaticMeshActor::Serialize(YAML::Emitter& Archive)
{
	Actor::Serialize(Archive);

	Archive << YAML::Key << "StaticMeshActor";
	Archive << YAML::BeginMap;
	{

	}
	Archive << YAML::EndMap;
}

void StaticMeshActor::Deserialize(YAML::Node& Archive)
{
	m_rootComponent = World::Get().DestroyComponent(m_rootComponent);

	Actor::Deserialize(Archive);

	if (YAML::Node node = Archive["StaticMeshActor"])
	{

	}
}
