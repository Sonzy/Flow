#pragma once
#include "Flow\GameFramework\Gameobject.h"
#include <vector>

namespace Flow
{
	class WorldObject;
	class WorldComponent;

	class FLOW_API Component : public GameObject
	{
	public:
		Component();
		Component(const std::string& Name);

		void AddChild(Component* Child);

		WorldComponent* GetParentComponent();
		WorldObject* GetParentWorldObject();

		void SetParent(WorldObject* Parent);

		virtual void Tick(float DeltaTime) override;

	protected:

		WorldObject* m_ParentObject;
	};
}