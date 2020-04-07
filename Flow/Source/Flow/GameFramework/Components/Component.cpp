#include "Flowpch.h"
#include "Component.h"

namespace Flow
{
	Component::Component()
		: Component("Unnamed Component")
	{
	}

	Component::Component(const std::string& Name)
		: GameObject(Name), ParentObject_(nullptr), AttachedComponent_(nullptr)
	{
	}

	Component::~Component()
	{
	}

	WorldComponent* Component::GetParentComponent()
	{
		return AttachedComponent_;
	}

	WorldObject* Component::GetParentWorldObject()
	{
		return ParentObject_;
	}

	void Component::SetParent(WorldObject* Parent)
	{
		ParentObject_ = Parent;
	}

	void Component::SetParentComponent(WorldComponent* Parent)
	{
		AttachedComponent_ = Parent;
	}

	void Component::Tick(float DeltaTime)
	{

	}
}


