#pragma once
#include "Flow\Logging\Log.h"
#include "Gameobject.h"
#include "Components\Component.h"
#include <memory>

namespace Flow
{
	class WorldComponent;

	class FLOW_API WorldObject : public GameObject
	{
	public:
		WorldObject();
		WorldObject(const std::string& Name);

		virtual void Tick(float DeltaTime) override;

		WorldComponent* GetRootComponent();

		void Render();

	protected:

		template <typename T>
		std::shared_ptr<T> CreateComponent(const std::string& NewName)
		{
			//TODO: Check if T is a component
			FLOW_ENGINE_WARNING("TODO: WorldObject::CreateComponent<T>: Check if T is a component");

			std::shared_ptr<T> NewComponent = std::make_shared<T>(NewName);
			Component* Comp = static_cast<Component*>(NewComponent.get());
			Comp->SetParent(this);
			return NewComponent;
		}

	protected:

		WorldComponent* m_RootComponent;
	};
}