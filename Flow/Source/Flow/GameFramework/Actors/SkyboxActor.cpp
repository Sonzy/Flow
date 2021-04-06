#include "pch.h"
#include "SkyboxActor.h"
#include "Flow/GameFramework/Components/SkyboxComponent.h"

#include <yaml-cpp/yaml.h>

SkyboxActor::SkyboxActor()
	: SkyboxActor("Skybox Actor")
{
}

SkyboxActor::SkyboxActor(const std::string& Name)
	: Actor(Name)
{
	m_Skybox = CreateComponent<SkyboxComponent>("Skybox");
	m_Skybox->SetOwned(false);
	m_RootComponent = m_Skybox;
}

void SkyboxActor::Serialize(YAML::Emitter& Archive)
{
	Actor::Serialize(Archive);

	Archive << YAML::Key << "SkyboxActor";
	Archive << YAML::BeginMap;
	{

	}
	Archive << YAML::EndMap;
}

void SkyboxActor::Deserialize(YAML::Node& Archive)
{
	m_RootComponent = World::Get().DestroyComponent(m_RootComponent);

	Actor::Deserialize(Archive);

	if (YAML::Node node = Archive["SkyboxActor"])
	{

	}
}