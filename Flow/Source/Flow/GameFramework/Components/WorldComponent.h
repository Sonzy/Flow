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
		virtual ~WorldComponent();

		virtual void Tick(float DeltaTime) override;

		void AddChild(WorldComponent* Child);

		Vector GetWorldLocation();
		Vector GetRelativeLocation();
		void SetWorldLocation(Vector NewLocation);
		void SetRelativeLocation(Vector NewLocation);

		Rotator GetWorldRotation();
		Rotator GetRelativeRotation();
		void SetWorldRotation(Rotator NewRotation);
		void SetRelativeRotation(Rotator NewRotation);

		Vector GetWorldScale();
		Vector GetRelativeScale();
		void SetWorldScale(Vector NewScale);
		void SetRelativeScale(Vector NewScale);

		Transform GetWorldTransform();
		Transform GetRelativeTransform();
		void SetWorldTransform(Transform NewTransform);
		void SetRelativeTransform(Transform NewTransform);




		virtual void Render();

	protected:

		std::vector<Component*> m_Children;
		Component* m_ParentComponent;

		Transform m_RelativeTransform;
	};
}