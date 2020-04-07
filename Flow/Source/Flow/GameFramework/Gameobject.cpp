#include "Flowpch.h"
#include "Gameobject.h"

Flow::GameObject::GameObject()
	: ObjectName_("Unnamed Object")
{
}

Flow::GameObject::GameObject(const std::string& Name)
	: ObjectName_(Name)
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
	return ObjectName_;
}
