#pragma once
#include "Flow\Logging\Log.h"
#include "Gameobject.h"
#include <memory>

namespace Flow
{
	class WorldComponent;

	class FLOW_API WorldObject : public GameObject
	{
	public:
		WorldObject();

		virtual void Tick(float DeltaTime) override;

	protected:

		template <typename T>
		std::shared_ptr<T> CreateComponent()
		{
			//TODO: Check if T is a component
			FLOW_ENGINE_LOG("TODO: WorldObject::CreateComponent<T>: Check if T is a component");
			return std::make_shared<T>();
		}

	protected:

		WorldComponent* m_RootComponent;
	};
}