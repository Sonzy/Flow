#include "pch.h"
#include "DirectionalLightActor.h"
#include "Flow/GameFramework/Components/Lights/DirectionalLightComponent.h"
#include "Flow/GameFramework/Components/WorldComponent.h"
#include "Flow/GameFramework/Components/Component.h"

#include <yaml-cpp/yaml.h>
#include "Framework/Utils/YamlSerializer.h"

DirectionalLightActor::DirectionalLightActor()
	: DirectionalLightActor("Directional Light Actor")
{
}

DirectionalLightActor::DirectionalLightActor(const std::string& Name)
	: Actor(Name)
{
	m_DirectionalLight = CreateComponent<DirectionalLightComponent>("Directional Light");
	m_DirectionalLight->SetOwned(false);
	m_rootComponent = m_DirectionalLight;
}

void DirectionalLightActor::Serialize(YAML::Emitter& Archive)
{
	Actor::Serialize(Archive);

	Archive << YAML::Key << "DirectionalLightActor";
	Archive << YAML::BeginMap;
	{

	}
	Archive << YAML::EndMap;
}

void DirectionalLightActor::Deserialize(YAML::Node& Archive)
{
	m_rootComponent = World::Get().DestroyComponent(m_rootComponent);

	Actor::Deserialize(Archive);

	if (YAML::Node node = Archive["DirectionalLightActor"])
	{

	}
}
