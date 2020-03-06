#include "Flowpch.h"
#include "World.h"
#include "WorldObject.h"

namespace Flow
{
	World::World()
		: m_WorldName("Unnamed World")
	{
	}

	World::World(const std::string& WorldName)
		: m_WorldName(WorldName)
	{
	}

	World::~World()
	{
		m_WorldObjects.clear();
	}

	void World::Tick(float DeltaTime)
	{
		for (auto& WorldObj : m_WorldObjects)
		{
			WorldObj->Tick(DeltaTime);
		}
	}
	const std::string& World::GetName()
	{
		return m_WorldName;
	}
}