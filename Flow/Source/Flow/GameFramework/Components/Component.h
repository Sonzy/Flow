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
		virtual ~Component();

		WorldComponent* GetParentComponent();
		WorldObject* GetParentWorldObject();

		void SetParent(WorldObject* Parent);
		void SetParentComponent(WorldComponent* Parent);

		virtual void Tick(float DeltaTime) override;

	protected:

		WorldObject* m_ParentObject;
		WorldComponent* m_AttachedComponent;
	};
}