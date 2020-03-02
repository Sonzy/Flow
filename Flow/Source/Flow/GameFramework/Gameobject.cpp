#include "Flowpch.h"
#include "Gameobject.h"

Flow::GameObject::GameObject()
	: m_ObjectName("Unnamed Object")
{
}

void Flow::GameObject::Tick(float DeltaTime)
{
}

const std::string& Flow::GameObject::GetName()
{
	return m_ObjectName;
}
