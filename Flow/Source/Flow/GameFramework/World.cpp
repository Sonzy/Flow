#include "Flowpch.h"
#include "World.h"
#include "WorldObject.h"

namespace Flow
{
	World::World()
	{
	}

	void World::Tick(float DeltaTime)
	{
		for (auto& WorldObj : m_WorldObjects)
		{
			WorldObj->Tick(DeltaTime);
		}
	}
}