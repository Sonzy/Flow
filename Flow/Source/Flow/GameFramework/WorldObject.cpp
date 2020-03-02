#include "Flowpch.h"
#include "WorldObject.h"
#include "Components\WorldComponent.h"

namespace Flow
{
	WorldObject::WorldObject()
		: m_RootComponent(nullptr)
	{

	}

	void WorldObject::Tick(float DeltaTime)
	{
		GameObject::Tick(DeltaTime);

		m_RootComponent->Tick(DeltaTime);
	}
	WorldComponent* WorldObject::GetRootComponent()
	{
		return m_RootComponent;
	}

	void WorldObject::Render()
	{
		m_RootComponent->Render();
	}
}