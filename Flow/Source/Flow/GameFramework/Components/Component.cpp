#include "Flowpch.h"
#include "Component.h"

namespace Flow
{
	Component::Component()
	{
	}

	void Component::AddChild(Component* Child)
	{
	}

	WorldComponent* Component::GetParentComponent()
	{
		return nullptr;
	}

	WorldObject* Component::GetParentWorldObject()
	{
		return nullptr;
	}

	void Component::SetParent(WorldObject* Parent)
	{
		m_ParentObject = Parent;
	}

	void Component::Tick(float DeltaTime)
	{

	}
}


