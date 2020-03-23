#pragma once
#include "Component.h"
#include "Flow\Helper\Maths.h"

class btRigidBody;

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

		virtual void InitialisePhysics();
		virtual btRigidBody* GetRigidBody();


		virtual void Render();

		//TODO: Probs not safe lool
		Vector* GetWriteablePosition();
		Rotator* GetWriteableRotation();
		Vector* GetWriteableScale();

		std::vector<Component*> GetChildren() const;

		void SetVisibility(bool Visible);
		bool IsVisible() const;

	protected:

		std::vector<Component*> m_Children;
		Component* m_ParentComponent;

		Transform m_RelativeTransform;

		bool m_Visible = true;
	};
}