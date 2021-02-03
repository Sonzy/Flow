#pragma once

//= Includes ========================================================

#include "Component.h"
#include "Maths/Maths.h"
#include <fstream>

//= Forward Declarations ============================================

class btRigidBody;
class btCollisionShape;
class MotionState;
class IconManager;
namespace YAML { class Emitter; }

//= Class Definitions ===============================================

class FLOW_API WorldComponent : public Component
{
public:

	REGISTER_CLASS(WorldComponent)
	
	//= Public Functions ================================

									WorldComponent();
									WorldComponent(const std::string& Name);
	virtual							~WorldComponent();

#if WITH_EDITOR
	virtual void					EditorBeginPlay() override;
	virtual void					OnViewportSelected() override;
	virtual void					OnViewportDeselected() override;

	virtual void					IconUpdate(IconManager& iconManager);
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
	void							AddRelativeScale(Vector3 NewScale);

	Transform						GetWorldTransform() const;
	Transform						GetRelativeTransform() const;
	void							SetWorldTransform(Transform NewTransform);
	void							SetRelativeTransform(Transform NewTransform);

	Matrix4x4						GetTransformationMatrix();

	virtual void					Render();

	Vector3*						GetWriteablePosition();
	Rotator*						GetWriteableRotation();
	Vector3*						GetWriteableScale();

	virtual void					DrawDetailsWindow(bool bDontUpdate) override;
	virtual void					DrawComponentDetailsWindow() override;
	void							DrawComponentInActorTreeRecursive();

	std::vector<WorldComponent*>	GetChildren() const;
	WorldComponent*					GetChildByName(const std::string& ChildName);
	void							BuildChildTree(std::vector<WorldComponent*>& Container, bool IncludeSelf = true); //Gets all components in the hierarchy tree

	void							SetVisibility(bool Visible);
	bool							IsVisible() const;

	//= Physics =

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

	//= Save/Load =

	virtual void					Serialize(YAML::Emitter& Archive) override;
	virtual void					Deserialize(YAML::Node& Archive)  override;

	virtual void					DefaultInitialise();

public:

	//= Public Variables ===================================

	std::string						m_Tag;

protected:

	//= Protected Variables ================================

	std::vector<WorldComponent*>	m_Children;
	WorldComponent*					m_ParentComponent;
	Transform						m_RelativeTransform;
	Maths::Matrix					m_matrix;
	bool							m_Visible = true;

	//= Physics ========

	bool							m_SimulatePhysics;
	bool							m_CollisionEnabled;
	btRigidBody*					m_RigidBody;
	btCollisionShape*				m_CollisionShape;
	MotionState*					m_MotionState;
};