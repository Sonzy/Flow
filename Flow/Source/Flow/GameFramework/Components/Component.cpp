#include "Flowpch.h"
#include "Component.h"

Component::Component()
	: Component("Unnamed Component")
{
}

Component::Component(const std::string& Name)
	: GameObject(Name), _ParentObject(nullptr), _AttachedComponent(nullptr)
{
}

Component::~Component()
{
}

WorldComponent* Component::GetParentComponent() const
{
	return _AttachedComponent;
}

WorldObject* Component::GetParentWorldObject() const
{
	return _ParentObject;
}

void Component::SetParent(WorldObject* Parent)
{
	_ParentObject = Parent;
}

void Component::SetParentComponent(WorldComponent* Parent)
{
	_AttachedComponent = Parent;
}

void Component::Tick(float DeltaTime)
{

}

void Component::DrawComponentDetailsWindow()
{
}


