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
		World(const std::string& WorldName);
		~World();

		template<typename T>
		std::shared_ptr<T> SpawnWorldObject()
		{
			std::shared_ptr<T> NewObject = std::make_shared<T>();
			m_WorldObjects.push_back(NewObject);

			return NewObject;
		}

		void Tick(float DeltaTime);

		const std::string& GetName();

	private:
		friend class Inspector;

		std::vector<std::shared_ptr<WorldObject>> m_WorldObjects;
		std::string m_WorldName;
	};
}