#include "Flowpch.h"
#include "Gameobject.h"
#include "Utils/GUIDGenerator.h"
#include <yaml-cpp/yaml.h>

GameObject::GameObject()
	: m_name("Unnamed Object")
{
}

GameObject::GameObject(const std::string& Name)
	: m_name(Name)
	, m_guid(-1)
{
}

GameObject::~GameObject()
{
}

void GameObject::Tick(float DeltaTime)
{
}

void GameObject::DrawDetailsWindow(bool bDontUpdate)
{
}

const std::string& GameObject::GetName() const
{
	return m_name;
}

std::string& GameObject::GetWritableName()
{
	return m_name;
}

void GameObject::SetName(const std::string& NewName)
{
	m_name = NewName;
}

void GameObject::Serialize(YAML::Emitter& Archive)
{
	Archive << YAML::Key << "GameObject";
	Archive << YAML::Value << YAML::BeginMap;
	
	Archive << YAML::Key << "Name" << YAML::Value << m_name.c_str();
	Archive << YAML::Key << "ClassName" << YAML::Value << GetClassNameID();
	Archive << YAML::Key << "Guid" << YAML::Value << m_guid;
	
	Archive << YAML::EndMap;
}

void GameObject::Deserialize(YAML::Node& Archive)
{
	YAML::Node GameObject = Archive["GameObject"];
	{
		m_guid = GameObject["Guid"].as<FGUID>();
		m_name = GameObject["Name"].as<std::string>();
	}
}
