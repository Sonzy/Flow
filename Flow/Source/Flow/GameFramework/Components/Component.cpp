#include "Flowpch.h"
#include "Component.h"

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

WorldComponent* Component::GetParentComponent() const
{
	return m_AttachedComponent;
}

Actor* Component::GetParentActor() const
{
	return m_ParentObject;
}

void Component::SetParent(Actor* Parent)
{
	m_ParentObject = Parent;
}

void Component::SetParentComponent(WorldComponent* Parent)
{
	m_AttachedComponent = Parent;
}

void Component::BeginPlay()
{

}

void Component::Tick(float DeltaTime)
{

}

void Component::DrawComponentDetailsWindow()
{
}


