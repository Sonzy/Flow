#pragma once
#include "Flow\Helper\Maths.h"

#include "Flow\Rendering\Core\Bindable.h"
#include "Flow\Rendering\Core\Bindables\IndexBuffer.h"
#include "Flow/GameFramework/Components/WorldComponent.h"
#include "Flow/GameFramework/WorldObject.h"

class btCollisionShape;
class btGhostObject;

class Mesh;
class Material;
class StaticMeshComponent;
class World;

enum class SelectedAxis
{
	None,
	X,
	Y,
	Z
};

/* Gizmo used for moving objects within the world within the editor */
class SelectionGizmo : public WorldObject
{
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

	/* Adds the collision data to the physics world. */
	void AddCollidersToWorld(World* World);

	void OnSelected(SelectedAxis SelectedAxis, WorldComponent* Object);
	void OnDeselected();

	SelectedAxis GetSelectedAxis() const { return _SelectedAxis; }

private:
	void GenerateCollisionData(StaticMeshComponent* Component, btCollisionShape*& Collider, btGhostObject*& Ghost);

private:

	StaticMeshComponent* _ArrowX;
	StaticMeshComponent* _ArrowY;
	StaticMeshComponent* _ArrowZ;

	Material* _ArrowMaterial;

	bool _Visible = false;

	SelectedAxis _SelectedAxis;
	IntVector2D _MouseLastUpdate;
	WorldComponent* _SelectedComponent;
	Vector _SelectedComponentStartPosition;
	float _MouseDistanceThreshold = 1.0f;
	Vector ArrowOffset;

	//= Collision ================

	btCollisionShape* _XCollision;
	btCollisionShape* _YCollision;
	btCollisionShape* _ZCollision;
	btGhostObject* _XGhost;
	btGhostObject* _YGhost;
	btGhostObject* _ZGhost;
};