#include "Flowpch.h"
#include "Gameobject.h"

GameObject::GameObject()
	: m_ObjectName("Unnamed Object")
{
}

GameObject::GameObject(const std::string& Name)
	: m_ObjectName(Name)
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
	return m_ObjectName;
}

std::string& GameObject::GetWritableName()
{
	return m_ObjectName;
}

void GameObject::SetName(const std::string& NewName)
{
	m_ObjectName = NewName;
}
