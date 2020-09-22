#pragma once
#include "Component.h"
#include "Maths/Maths.h"
#include <fstream>

class btRigidBody;
class btCollisionShape;
class MotionState;

class FLOW_API WorldComponent : public Component
{
public:
									WorldComponent();
									WorldComponent(const std::string& Name);
	virtual							~WorldComponent();

#if WITH_EDITOR
	virtual void					EditorBeginPlay() override;
	virtual void					OnViewportSelected() override;
	virtual void					OnViewportDeselected() override;
#endif
	virtual void					BeginPlay() override;
	virtual void					Tick(float DeltaTime) override;

	void							AddChild(WorldComponent* Child);

	Vector3							GetWorldPosition() const;
	Vector3							GetRelativePosition() const;
	void							SetWorldPosition(Vector3 NewPosition);
	void							SetRelativePosition(Vector3 NewPosition);
	void							AddRelativePosition(Vector3 Position);

	Rotator							GetWorldRotation() const;
	Rotator							GetRelativeRotation() const;
	void							SetWorldRotation(Rotator NewRotation);
	void							SetRelativeRotation(Rotator NewRotation);
	void							AddRelativeRotation(Rotator Rotation);

	Vector3							GetWorldScale() const;
	Vector3							GetRelativeScale() const;
	void							SetWorldScale(Vector3 NewScale);
	void							SetRelativeScale(Vector3 NewScale);

	Transform						GetWorldTransform() const;
	Transform						GetRelativeTransform() const;
	void							SetWorldTransform(Transform NewTransform);
	void							SetRelativeTransform(Transform NewTransform);

	virtual void					Render();

	Vector3*							GetWriteablePosition();
	Rotator*						GetWriteableRotation();
	Vector3*							GetWriteableScale();

	void							DrawInspectionTree(WorldComponent* CurrentInspectedComponent, bool DontOpenTree = false);
	virtual void					DrawDetailsWindow(bool bDontUpdate) override;

	std::vector<WorldComponent*>	GetChildren() const;
	WorldComponent*					GetChildByName(const std::string& ChildName);

	void							SetVisibility(bool Visible);
	bool							IsVisible() const;

	//= Physics =====

	virtual void					InitialisePhysics();
	virtual void					DestroyPhysics();
	void							CreateRigidBody();
	void							UpdateAABB();

	btRigidBody*					GetRigidBody() const;
	btCollisionShape*				GetCollisionShape() const;

	void							SetSimulatePhysics(bool Simulate);
	bool							IsSimulatingPhysics() const;
	bool							HasCollision() const;
	void							SetCollisionEnabled(bool Enabled) { m_CollisionEnabled = Enabled; }

	void							UpdateCollisionScale();

protected:

	std::vector<WorldComponent*>	_Children;
	WorldComponent*					_ParentComponent;

	Transform						_RelativeTransform;

	bool							_Visible = true;

	//= Physics ========

	bool							_SimulatePhysics;
	bool							m_CollisionEnabled;
	btRigidBody*					_RigidBody;
	btCollisionShape*				m_CollisionShape;
	MotionState*					_MotionState;

public:
	std::string						_Tag;

public:

	virtual							std::string GetClassSerializationUID(std::ofstream* Archive);
	virtual void					Serialize(std::ofstream* Archive);
	void							SerializeChildren(std::ofstream* Archive);
	virtual void					Deserialize(std::ifstream* Archive, Actor* NewParent);
	void							DeserializeChildren(std::ifstream* Archive, Actor* NewParent);
};