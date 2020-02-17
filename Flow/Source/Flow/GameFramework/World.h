#pragma once
#include "Flow\Core.h"
#include <vector>
#include <memory>

namespace Flow
{
	class WorldObject;

	class FLOW_API World
	{
	public:
		World();

		template<typename T>
		std::shared_ptr<T> SpawnWorldObject()
		{
			std::shared_ptr<T> NewObject = std::make_shared<T>();
			m_WorldObjects.push_back(NewObject);

			return NewObject;
		}

		void Tick(float DeltaTime);

	private:

		std::vector<std::shared_ptr<WorldObject>> m_WorldObjects;
	};
}