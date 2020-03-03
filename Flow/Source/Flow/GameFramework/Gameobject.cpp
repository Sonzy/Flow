#include "Flowpch.h"
#include "Gameobject.h"

Flow::GameObject::GameObject()
	: m_ObjectName("Unnamed Object")
{
}

Flow::GameObject::GameObject(const std::string& Name)
	: m_ObjectName(Name)
{
}

void Flow::GameObject::Tick(float DeltaTime)
{
}

const std::string& Flow::GameObject::GetName()
{
	return m_ObjectName;
}
