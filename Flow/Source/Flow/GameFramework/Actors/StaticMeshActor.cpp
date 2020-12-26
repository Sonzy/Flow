#include "Flowpch.h"
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
	m_RootComponent = m_MeshComponent;
}

StaticMeshActor::~StaticMeshActor()
{
	delete m_MeshComponent;
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
		//TODO: Dont actually save this? Just use a normal actor with a component?
		Archive << YAML::Key << "MeshName" << YAML::Value << ((m_MeshComponent != nullptr) ? m_MeshComponent->GetMeshIdentifier() : "");
		Archive << YAML::Key << "MaterialName" << YAML::Value << ((m_MeshComponent != nullptr) ? m_MeshComponent->GetMaterialIdentifier() : "");
	}
	Archive << YAML::EndMap;
}

void StaticMeshActor::Deserialize(YAML::Node& Archive)
{
	Actor::Deserialize(Archive);

	YAML::Node node = Archive["StaticMeshActor"];
	m_MeshComponent->SetMeshAndMaterial(node["MeshName"].as<std::string>(), node["MaterialName"].as<std::string>());
}
