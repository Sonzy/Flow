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

	Component* Component::GetParentComponent()
	{
		return nullptr;
	}

	WorldObject* Component::GetParentWorldObject()
	{
		return nullptr;
	}

	void Component::Tick(float DeltaTime)
	{
		for (auto& Child : m_Children)
		{
			Child->Tick(DeltaTime);
		}
	}
}


