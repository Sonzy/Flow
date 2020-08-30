#pragma once
#include "Flow\Helper\Maths.h"

#include "Flow\Rendering\Core\Bindable.h"
#include "Flow\Rendering\Core\Bindables\IndexBuffer.h"
#include "Flow/GameFramework/Components/WorldComponent.h"
#include "Flow/GameFramework/Actor.h"

class btCollisionShape;
class btGhostObject;

class Mesh;
class Material;
class StaticMeshComponent;
class World;
class MeshAsset;

enum class Axis
{
	None,
	X,
	Y,
	Z
};

/* Gizmo used for moving objects within the world within the editor */
class SelectionGizmo : public Actor
{
public:
	enum Transform
	{
		Translation,
		Rotation,
		Scale
	};

public:
	SelectionGizmo();
	virtual ~SelectionGizmo();

	/* Generates the collision for all parts of the gizmo*/
	void GenerateCollision();

	void InitialisePhysics();

	void UpdateSelection();

	void UpdatePosition(Vector Position);
	void UpdateRotation(Rotator Rotation);
	void SetScale(Vector Scale);

	void Render();

	[[nodiscard]] Vector GetPosition()  const;

	void SetVisibility(bool Visible);
	bool IsVisible() const { return _Visible; }

	/* Adds the collision data to the physics world. */
	void AddCollidersToWorld(World* World);	
	void RemoveCollidersFromWorld(World* World);	
	void Reset();

	void OnSelected(Axis SelectedAxis, WorldComponent* Object);
	void OnNewComponentSelected(WorldComponent* Object);
	void OnDeselected();

	Axis GetSelectedAxis() const { return m_SelectedAxis; }

	StaticMeshComponent* GetArrow(Axis axis) const;;
	btCollisionShape* GetArrowCollision(Axis axis) const;

	SelectionGizmo::Transform GetTransformationMode() const				{ return m_TransformMode; }
	void SetTransformationMode(SelectionGizmo::Transform newMode);

private:
	void GenerateCollisionData(StaticMeshComponent* Component, btCollisionShape*& Collider, btGhostObject*& Ghost);

private:

	WorldComponent*					m_Root;
	StaticMeshComponent*			m_ArrowX;
	StaticMeshComponent*			m_ArrowY;
	StaticMeshComponent*			m_ArrowZ;

	bool							m_Visible = false;

	SelectionGizmo::Transform		m_TransformMode;
	Axis							m_SelectedAxis;
	IntVector2D						m_MouseLastUpdate;
	WorldComponent*					m_SelectedComponent;
	Vector							m_SelectedComponentStartPosition;
	Vector							m_SelectedComponentStartScale;
	float							m_MouseDistanceThreshold = 1.0f;
	Vector							m_ArrowOffset;

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

	btCollisionShape*				m_XCollision;
	btCollisionShape*				m_YCollision;
	btCollisionShape*				m_ZCollision;
	btGhostObject*					m_XGhost;
	btGhostObject*					m_YGhost;
	btGhostObject*					m_ZGhost;
};