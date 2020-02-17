#pragma once
#include "Flow\GameFramework\Gameobject.h"
#include <vector>

namespace Flow
{
	class WorldObject;

	class FLOW_API Component : public GameObject
	{
	public:
		Component();

		void AddChild(Component* Child);

		Component* GetParentComponent();
		WorldObject* GetParentWorldObject();

		virtual void Tick(float DeltaTime) override;

	protected:

		Component* m_ParentComponent;
		std::vector<Component*> m_Children;
	};
}