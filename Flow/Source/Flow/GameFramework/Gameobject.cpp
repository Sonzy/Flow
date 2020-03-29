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

Flow::GameObject::~GameObject()
{
}

void Flow::GameObject::Tick(float DeltaTime)
{
}

void Flow::GameObject::DrawDetailsWindow(bool bDontUpdate)
{
}

const std::string& Flow::GameObject::GetName()
{
	return m_ObjectName;
}
