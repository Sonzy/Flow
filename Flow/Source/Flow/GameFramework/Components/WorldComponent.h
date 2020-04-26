#pragma once
#include "Component.h"
#include "Flow\Helper\Maths.h"
#include <functional>

class btRigidBody;
class btCollisionShape;
class btMotionState;

namespace Flow
{
	enum class PhysicsMode
	{
		Disabled,
		Static,
		Dynamic,
		Kinematic
	};

	//TODO: Calls on copy :C
	//struct Delegate
	//{
	//	void Call(WorldComponent* Obj1, WorldComponent* Obj2)
	//	{
	//		for (auto& Func : Functions)
	//		{
	//			Func(Obj1, Obj2);
	//		}
	//	}
	//
	//	bool IsBound()
	//	{
	//		return Functions.size() > 0;
	//	}
	//
	//	void Bind(std::function<void(Flow::WorldComponent*, Flow::WorldComponent*)> NewFunc)
	//	{
	//		Functions.push_back(NewFunc);
	//	}
	//
	//	std::list<std::function<void(Flow::WorldComponent*, Flow::WorldComponent*)>> Functions;
	//};

	class FLOW_API WorldComponent : public Component
	{
	public:
		WorldComponent();
		WorldComponent(const std::string& Name);
		virtual ~WorldComponent();

		virtual void BeginPlay() override;
		virtual void Tick(float DeltaTime) override;

		void AddChild(WorldComponent* Child);

		Vector GetWorldPosition() const;
		Vector GetRelativePosition() const;
		void SetWorldPosition(Vector NewPosition);
		void SetRelativePosition(Vector NewPosition);
		void AddRelativePosition(Vector Position);

		Rotator GetWorldRotation() const;
		Rotator GetRelativeRotation() const;
		void SetWorldRotation(Rotator NewRotation);
		void SetRelativeRotation(Rotator NewRotation);
		void AddRelativeRotation(Rotator Rotation);

		Vector GetWorldScale() const;
		Vector GetRelativeScale() const;
		void SetWorldScale(Vector NewScale);
		void SetRelativeScale(Vector NewScale);

		Transform GetWorldTransform() const;
		Transform GetRelativeTransform() const;
		void SetWorldTransform(Transform NewTransform);
		void SetRelativeTransform(Transform NewTransform);

		virtual bool InitialisePhysics(PhysicsMode Mode);
		virtual void InitialiseSubComponentPhysics(PhysicsMode Mode);
		virtual bool CreateCollision();
		virtual bool CreateRigidBody();
		virtual void MovePhysicsBody(Transform NewTransform);
		void UpdatePhysicsBody(bool ForceUpdate = false);
		btRigidBody* GetRigidBody() const;
		btCollisionShape* GetCollision() const;
		float GetMass() const;

		void SetObjectGravity(Vector NewGravity);
		void DisableGravity();
		void OverwriteWorldGravity(bool Overwrite);
		bool UsingCustomGravity() const;
		Vector GetGravity() const;

		virtual void Render();

		//TODO: Probs not safe lool
		Vector* GetWriteablePosition();
		Rotator* GetWriteableRotation();
		Vector* GetWriteableScale();

		void DrawInspectionTree(WorldComponent* CurrentInspectedComponent, bool DontOpenTree = false);
		virtual void DrawDetailsWindow(bool bDontUpdate) override;

		std::vector<WorldComponent*> GetChildren() const;

		void SetVisibility(bool Visible);
		bool IsVisible() const;

	protected:

		std::vector<WorldComponent*> Children_;
		Component* ParentComponent_;



		//= Physics Properties ===
		PhysicsMode PhysicsMode_;
		btCollisionShape* Collision_;
		btRigidBody* Rigidbody_;
		btMotionState* MotionState_;
		bool OverwriteWorldGravity_;
		Vector ObjectGravity_;
		float Mass_ = 1.0f;

		//= Transformation properties ===
		Transform RelativeTransform_;

		//= Rendering Information ===
		bool Visible_ = true;
	};
}