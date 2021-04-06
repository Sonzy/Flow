#include "pch.h"
#include "Component.h"
#include "GameFramework/Actor.h"
#include "GameFramework/World.h"
#include "Utils/GUIDGenerator.h"
#include <yaml-cpp/yaml.h>

Component::Component()
	: Component("Unnamed Component")
{
}

Component::Component(const std::string& Name)
	: GameObject(Name)
	, m_ParentObject(nullptr)
	, m_AttachedComponent(nullptr)
	, m_registered(false)
{
}

Component::~Component()
{
}

void Component::OnRegistered()
{
	m_registered = true;
}

bool Component::IsRegistered() const
{
	return m_registered;
}

WorldComponent* Component::GetParentComponent() const
{
	return m_AttachedComponent;
}

Actor* Component::GetParentActor() const
{
	return m_ParentObject;
}

void Component::SetParent(Actor* Parent)
{
	m_ParentObject = Parent;
}

void Component::SetParentComponent(WorldComponent* Parent)
{
	m_AttachedComponent = Parent;
}

void Component::BeginPlay()
{

}

void Component::Tick(float DeltaTime)
{

}

void Component::DrawComponentDetailsWindow()
{

}

void Component::Serialize(YAML::Emitter& Archive)
{
	GameObject::Serialize(Archive);

	Archive << YAML::Key << "Component";
	Archive << YAML::BeginMap;
	{
		Archive << YAML::Key << "ParentActor";
		Archive << YAML::Value << (m_ParentObject != nullptr ? m_ParentObject->GetGuid() : -1);
	}
	Archive << YAML::EndMap;
}

void Component::Deserialize(YAML::Node& Archive)
{
	GameObject::Deserialize(Archive);

	if (YAML::Node node = Archive["Component"])
	{
		m_ParentObject = World::Get().FindActor(node["ParentActor"].as<FGUID>());
	}
}


