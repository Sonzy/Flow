#include "Flowpch.h"
#include "PointLightActor.h"
#include "Flow/GameFramework/Components/PointLightComponent.h"
#include "Flow/GameFramework/Components/WorldComponent.h"
#include "Flow/GameFramework/Components/Component.h"

#include <yaml-cpp/yaml.h>
#include "Utils/YamlSerializer.h"

PointLightActor::PointLightActor()
	: PointLightActor("Point Light Actor")
{
}

PointLightActor::PointLightActor(const std::string& Name)
	: Actor(Name)
{
	m_PointLight = CreateComponent<PointLightComponent>("Point Light");
	m_PointLight->SetOwned(false);
	m_RootComponent = m_PointLight;
}

void PointLightActor::Serialize(YAML::Emitter& Archive)
{
	Actor::Serialize(Archive);

	Archive << YAML::Key << "PointLightActor";
	Archive << YAML::BeginMap;
	{

	}
	Archive << YAML::EndMap;
}

void PointLightActor::Deserialize(YAML::Node& Archive)
{
	m_RootComponent = World::Get()->DestroyComponent(m_RootComponent);

	Actor::Deserialize(Archive);

	if (YAML::Node node = Archive["PointLightActor"])
	{

	}
}
