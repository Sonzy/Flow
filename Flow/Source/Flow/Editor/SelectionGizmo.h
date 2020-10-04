#pragma once

//= Includes ======================================

#include "Maths/Vector3.h"
#include "Maths/IntVector2.h"
#include "Rendering\Core\Bindable.h"
#include "Rendering\Core\Bindables\IndexBuffer.h"
#include "GameFramework/Components/WorldComponent.h"
#include "GameFramework/Actor.h"

//= Forward Declarations ==========================

class btCollisionShape;
class btGhostObject;

class Mesh;
class Material;
class StaticMeshComponent;
class World;
class MeshAsset;

//= Global Enums ===================================

enum class Axis
{
	None,
	X,
	Y,
	Z
};

//= Class Definition ================================

/* Gizmo used for moving objects within the world within the editor */
class SelectionGizmo : public Actor
{
public:

	//= Public Enums ===============================
	enum Transform
	{
		Translation,
		Rotation,
		Scale
	};

public:

	//= Public Functions ===========================

								SelectionGizmo();
	virtual						~SelectionGizmo();

	void						InitialisePhysics();

	void						UpdateSelection();

	void						UpdatePosition(Vector3 Position);
	void						UpdateRotation(Rotator Rotation);
	void						SetScale(Vector3 Scale);

	void						Render();

	Vector3						GetPosition()  const;

	void						SetVisibility(bool Visible);
	bool						IsVisible() const { return m_Visible; }

	/* Adds the collision data to the physics world. */
	void						AddCollidersToWorld(World* World);	
	void						RemoveCollidersFromWorld(World* World);	
	void						Reset();

	void						OnSelected(Axis SelectedAxis, WorldComponent* Object);
	void						OnNewComponentSelected(WorldComponent* Object);
	void						OnDeselected();

	Axis						GetSelectedAxis() const { return m_SelectedAxis; }

	SelectionGizmo::Transform	GetTransformationMode() const				{ return m_TransformMode; }
	void						SetTransformationMode(SelectionGizmo::Transform newMode);

private:

	//= Private Functions =============================================

	void						GenerateCollision();

private:

	//= Private Variables =============================================

	WorldComponent*					m_Root;
	StaticMeshComponent*			m_ArrowX;
	StaticMeshComponent*			m_ArrowY;
	StaticMeshComponent*			m_ArrowZ;

	bool							m_Visible = false;

	SelectionGizmo::Transform		m_TransformMode;
	Axis							m_SelectedAxis;
	IntVector2						m_MouseLastUpdate;
	WorldComponent*					m_SelectedComponent;
	Vector3							m_SelectedComponentStartPosition;
	Vector3							m_SelectedComponentStartScale;
	float							m_MouseDistanceThreshold = 1.0f;
	Vector3							m_ArrowOffset;

	//= Defaults =================

	Rotator							m_Translation_X_Rot;
	Rotator							m_Translation_Y_Rot;
	Rotator							m_Translation_Z_Rot;

	Rotator							m_Rotation_X_Rot;
	Rotator							m_Rotation_Y_Rot;
	Rotator							m_Rotation_Z_Rot;

	//= Meshes ===================

	MeshAsset*						m_MeshTranslate;
	MeshAsset*						m_MeshRotate;
	MeshAsset*						m_MeshScale;

	//= Collision ================

	btConvexHullShape				m_CollisionTranslation;
	btConvexHullShape				m_CollisionRotation;
	btConvexHullShape				m_CollisionScale;
	btGhostObject*					m_XGhost;
	btGhostObject*					m_YGhost;
	btGhostObject*					m_ZGhost;
};