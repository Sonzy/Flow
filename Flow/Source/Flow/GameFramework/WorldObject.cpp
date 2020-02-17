#include "Flowpch.h"
#include "WorldObject.h"
#include "Components\WorldComponent.h"

namespace Flow
{
	WorldObject::WorldObject()
	{

	}

	void WorldObject::Tick(float DeltaTime)
	{
		GameObject::Tick(DeltaTime);

		m_RootComponent->Tick(DeltaTime);
	}
}