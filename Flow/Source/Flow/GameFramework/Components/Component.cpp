#include "Flowpch.h"
#include "Component.h"

namespace Flow
{
	Component::Component()
		: Component("Unnamed Component")
	{
	}

	Component::Component(const std::string& Name)
		: GameObject(Name), m_ParentObject(nullptr), m_AttachedComponent(nullptr)
	{
	}

	Component::~Component()
	{
	}

	WorldComponent* Component::GetParentComponent()
	{
		return m_AttachedComponent;
	}

	WorldObject* Component::GetParentWorldObject()
	{
		return m_ParentObject;
	}

	void Component::SetParent(WorldObject* Parent)
	{
		m_ParentObject = Parent;
	}

	void Component::SetParentComponent(WorldComponent* Parent)
	{
		m_AttachedComponent = Parent;
	}

	void Component::Tick(float DeltaTime)
	{

	}
}


