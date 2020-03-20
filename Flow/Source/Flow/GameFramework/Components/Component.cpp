#include "Flowpch.h"
#include "Component.h"

namespace Flow
{
	Component::Component()
		: GameObject("Unnamed Component")
	{
	}

	Component::Component(const std::string& Name)
		: GameObject(Name)
	{
	}

	Component::~Component()
	{
	}

	WorldComponent* Component::GetParentComponent()
	{
		return nullptr;
	}

	WorldObject* Component::GetParentWorldObject()
	{
		return m_ParentObject;
	}

	void Component::SetParent(WorldObject* Parent)
	{
		m_ParentObject = Parent;
	}

	void Component::Tick(float DeltaTime)
	{

	}
}


