#include "Flowpch.h"
#include "Gameobject.h"

GameObject::GameObject()
	: _ObjectName("Unnamed Object")
{
}

GameObject::GameObject(const std::string& Name)
	: _ObjectName(Name)
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

const std::string& GameObject::GetName()
{
	return _ObjectName;
}
