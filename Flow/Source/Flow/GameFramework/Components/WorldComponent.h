#pragma once
#include "Component.h"
#include "Flow\Helper\Maths.h"

namespace Flow
{
	class FLOW_API WorldComponent : public Component
	{
	public:
		WorldComponent();
		WorldComponent(const std::string& Name);

		virtual void Tick(float DeltaTime) override;

		Vector GetWorldLocation();
		void SetWorldLocation(Vector& NewLocation);
		void SetRelativeLocation(const Vector& NewLocation);

		virtual void Render();

	protected:

		std::vector<Component*> m_Children;
		Component* m_ParentComponent;
		Vector m_RelativeLocation;
	};
}