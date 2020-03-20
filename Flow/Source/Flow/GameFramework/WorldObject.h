#pragma once
#include "Flow\Logging\Log.h"
#include "Gameobject.h"
#include "Components\Component.h"
#include <memory>
#include <type_traits>

#include "btBulletCollisionCommon.h"

namespace Flow
{
	class WorldComponent;

	class FLOW_API WorldObject : public GameObject
	{
	public:
		WorldObject();
		WorldObject(const std::string& Name);
		virtual ~WorldObject();

		virtual void Tick(float DeltaTime) override;

		WorldComponent* GetRootComponent();

		void Render();

		bool IsSimulatingPhysics();
		bool CollisionEnabled();

		virtual void InitialisePhysics();
		btRigidBody* GetRigidBody();

		virtual void DrawDetailsWindow() override;


	protected:

		/* Creates a new component, assigns it's parent as this object and returns it */
		template <typename T>
		std::shared_ptr<T> CreateComponent(const std::string& NewName)
		{
			static_assert(std::is_base_of<Component, T>::value, "Tried to create a component templated with a non-component type");

			std::shared_ptr<T> NewComponent = std::make_shared<T>(NewName);
			Component* Comp = static_cast<Component*>(NewComponent.get());
			Comp->SetParent(this);
			return NewComponent;
		}

	protected:

		WorldComponent* m_RootComponent;

		//= Physics ===================

		bool SimulatePhysics = false;
		bool HasCollision = false;
		int Tag;
	};
}