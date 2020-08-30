#include "Flowpch.h"
#include "SelectionGizmo.h"

#include "Flow\Rendering\Renderer.h"
#include "Flow\Assets\Meshes\MeshAsset.h"
#include "Flow\GameFramework\Components\StaticMeshComponent.h"

#include "Flow/GameFramework/World.h"
#include "Flow\Assets\AssetSystem.h"

#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"
#include "BulletCollision\CollisionDispatch\btGhostObject.h"

#include "Flow/Input/Input.h"

#include "Flow/Helper/Maths.h"

#include "Flow/Editor/EditorLayer.h"

SelectionGizmo::SelectionGizmo()
	: Actor("SelectionGizmo")
	, m_Translation_X_Rot(Rotator(0.0f, -90.0f, 0.0f))
	, m_Translation_Y_Rot(Rotator(0.0f, 0.0f, 0.0f))
	, m_Translation_Z_Rot(Rotator(90.0f, 0.0f, 0.0f))
	, m_Rotation_X_Rot(Rotator(0.0f, 0.0f, -90.0f))
	, m_Rotation_Y_Rot(Rotator(90.0f, 0.0f, 0.0f))
	, m_Rotation_Z_Rot(Rotator(0.0f, 0.0f, 0.0f))
{
	m_MeshTranslate = AssetSystem::GetAsset<MeshAsset>("Gizmo_Translation");
	m_MeshRotate = AssetSystem::GetAsset<MeshAsset>("Gizmo_Rotation");
	m_MeshScale = AssetSystem::GetAsset<MeshAsset>("Gizmo_Scale");

	//Create and assign mesh components.
	m_Root = new WorldComponent("Root");
	m_ArrowX = new StaticMeshComponent("m_ArrowX", "Gizmo_Translation", "Mat_FlatColour_Red");
	m_ArrowY = new StaticMeshComponent("m_ArrowY", "Gizmo_Translation", "Mat_FlatColour_Green");
	m_ArrowZ = new StaticMeshComponent("m_ArrowZ", "Gizmo_Translation", "Mat_FlatColour_Blue");

	m_ArrowX->SetCollisionEnabled(false);
	m_ArrowY->SetCollisionEnabled(false);
	m_ArrowZ->SetCollisionEnabled(false);

	m_ArrowX->_Tag = "ArrowX";
	m_ArrowY->_Tag = "ArrowY";
	m_ArrowZ->_Tag = "ArrowZ";

	_RootComponent = m_Root;
	_RootComponent->AddChild(m_ArrowX);
	_RootComponent->AddChild(m_ArrowY);
	_RootComponent->AddChild(m_ArrowZ);

	m_ArrowX->SetParent(this);
	m_ArrowY->SetParent(this);
	m_ArrowZ->SetParent(this);

	m_ArrowX->SetWorldRotation(m_Translation_X_Rot);
	m_ArrowY->SetWorldRotation(m_Translation_Y_Rot);
	m_ArrowZ->SetWorldRotation(m_Translation_Z_Rot);

	//Init the transform
	const int Scale = 1.0f;
	m_ArrowX->SetWorldScale(Scale);
	m_ArrowY->SetWorldScale(Scale);
	m_ArrowZ->SetWorldScale(Scale);

	//Initialise the meshes
	m_ArrowX->RefreshBinds();
	m_ArrowY->RefreshBinds();
	m_ArrowZ->RefreshBinds();

	m_ArrowX->SetStencilMode(StencilMode::AlwaysOnTop);
	m_ArrowY->SetStencilMode(StencilMode::AlwaysOnTop);
	m_ArrowZ->SetStencilMode(StencilMode::AlwaysOnTop);

	m_ArrowX->m_DrawWithoutDepth = true;
	m_ArrowY->m_DrawWithoutDepth = true;
	m_ArrowZ->m_DrawWithoutDepth = true;
}

SelectionGizmo::~SelectionGizmo()
{
	delete m_XGhost;
	delete m_YGhost;
	delete m_ZGhost;

	delete m_ArrowX;
	delete m_ArrowY;
	delete m_ArrowZ;

	delete m_XCollision;
	delete m_YCollision;
	delete m_ZCollision;
}


void SelectionGizmo::GenerateCollision()
{
	m_XGhost = new btGhostObject();
	m_YGhost = new btGhostObject();
	m_ZGhost = new btGhostObject();

	//Create collision data for each arrow seperately
	GenerateCollisionData(m_ArrowX, m_XCollision, m_XGhost);
	GenerateCollisionData(m_ArrowY, m_YCollision, m_YGhost);
	GenerateCollisionData(m_ArrowZ, m_ZCollision, m_ZGhost);

	m_XGhost->setCollisionFlags(btGhostObject::CF_NO_CONTACT_RESPONSE);
	m_YGhost->setCollisionFlags(btGhostObject::CF_NO_CONTACT_RESPONSE);
	m_ZGhost->setCollisionFlags(btGhostObject::CF_NO_CONTACT_RESPONSE);
}

void SelectionGizmo::InitialisePhysics()
{
	//TODO: Remove
	//m_ArrowX->InitialisePhysics();
	//m_ArrowY->InitialisePhysics();
	//m_ArrowZ->InitialisePhysics();
}

void SelectionGizmo::UpdateSelection()
{
	if (m_SelectedAxis == Axis::None)
	{
		if (m_SelectedComponent)
			UpdatePosition(m_SelectedComponent->GetWorldPosition());
			
		return;
	}

	IntVector2D MousePosition = Input::GetMousePosition();
	IntVector2D MouseDifference = MousePosition - m_MouseLastUpdate;
	bool Add = MouseDifference.X > 0;

	if (MousePosition.Distance(m_MouseLastUpdate) < m_MouseDistanceThreshold)
		return;

	Vector AxisDirection;
	switch (m_SelectedAxis)
	{
	case Axis::X:
		AxisDirection = Vector(1.0f, 0.0f, 0.0f);
		break;
	case Axis::Y:
		AxisDirection = Vector(0.0f, 1.0f, 0.0f);
		break;
	case Axis::Z:
		AxisDirection = Vector(0.0f, 0.0f, 1.0f);
		break;
	}

	//Fire a ray from the current mouse position.
	Vector Start = RenderCommand::GetMainCamera()->GetCameraPosition();
	Vector Direction = RenderCommand::GetScreenToWorldDirectionVector(MousePosition.X, MousePosition.Y, EditorLayer::GetEditor()->GetSceneWindowSize(), EditorLayer::GetEditor()->GetSceneWindowPosition());

	switch (m_TransformMode)
	{
	case SelectionGizmo::Translation:
	{
		//Get the closest point on the axis line to the ray
		float AxisClosestScale;
		float RayClosestScale;
		Math::GetClosestDistanceBetweenLines(Ray(m_SelectedComponentStartPosition /*TODO: + the distance in axis where mouse originally clicked */, AxisDirection), Ray(Start, Direction), AxisClosestScale, RayClosestScale);

		Vector NewPosition = m_SelectedComponentStartPosition + (-AxisDirection * AxisClosestScale) + m_ArrowOffset;

		//Original Position + Closest point on x axis to mouse + Offset from object to arrow
		m_SelectedComponent->SetWorldPosition(NewPosition);
		UpdatePosition(NewPosition);

		m_MouseLastUpdate = MousePosition;

		//TODO: Make the collision were moving kinematic - or disable movement on non-kinematic object - or dont allow deactivation whilst moving
		// Update the bounding box of selected component whilst moving it
		if (m_SelectedComponent)
			m_SelectedComponent->UpdateAABB();

		break;
	}
	case SelectionGizmo::Rotation:
		break;
	case SelectionGizmo::Scale:
	{
		//Get the closest point on the axis line to the ray
		float AxisClosestScale;
		float RayClosestScale;
		Math::GetClosestDistanceBetweenLines(Ray(m_SelectedComponentStartPosition /*TODO: + the distance in axis where mouse originally clicked */, AxisDirection), Ray(Start, Direction), AxisClosestScale, RayClosestScale);

		Vector NewPosition = m_SelectedComponentStartPosition + (-AxisDirection * AxisClosestScale) + m_ArrowOffset;

		//TODO:  Update
		const float scaleSensitivity = 1.0f;
		float diffInAxis = 0;
		switch (m_SelectedAxis)
		{
		case Axis::X: diffInAxis = NewPosition.X - m_SelectedComponentStartPosition.X;	break;
		case Axis::Y: diffInAxis = NewPosition.Y - m_SelectedComponentStartPosition.Y;	break;
		case Axis::Z: diffInAxis = NewPosition.Z - m_SelectedComponentStartPosition.Z;	break;
		}

		Vector prevScale = m_SelectedComponent->GetWorldScale();
		switch (m_SelectedAxis)
		{
		case Axis::X: m_SelectedComponent->SetWorldScale(Vector(m_SelectedComponentStartScale.X + (diffInAxis * scaleSensitivity), prevScale.Y, prevScale.Z));	break;
		case Axis::Y: m_SelectedComponent->SetWorldScale(Vector(prevScale.X, m_SelectedComponentStartScale.Y + (diffInAxis * scaleSensitivity), prevScale.Z));	break;
		case Axis::Z: m_SelectedComponent->SetWorldScale(Vector(prevScale.X, prevScale.Y, m_SelectedComponentStartScale.Z + (diffInAxis * scaleSensitivity)));	break;
		}

		m_MouseLastUpdate = MousePosition;
		break;
	}
	}
}

void SelectionGizmo::UpdatePosition(Vector Position)
{
	m_ArrowX->SetWorldPosition(Position);
	m_ArrowY->SetWorldPosition(Position);
	m_ArrowZ->SetWorldPosition(Position);

	float degToRad = (Math::PI / 180.0f);

	btTransform Transform;
	Transform.setOrigin(btVector3(Position.X, Position.Y, Position.Z));

	switch (m_TransformMode)
	{
	case SelectionGizmo::Scale:
	case SelectionGizmo::Translation:

		Transform.setRotation(btQuaternion(m_Translation_X_Rot.Yaw * degToRad, m_Translation_X_Rot.Pitch * degToRad, m_Translation_X_Rot.Roll * degToRad));
		m_XGhost->setWorldTransform(Transform);
		Transform.setRotation(btQuaternion(m_Translation_Y_Rot.Yaw * degToRad, m_Translation_Y_Rot.Pitch * degToRad, m_Translation_Y_Rot.Roll * degToRad));
		m_YGhost->setWorldTransform(Transform);
		Transform.setRotation(btQuaternion(m_Translation_Z_Rot.Yaw * degToRad, m_Translation_Z_Rot.Pitch * degToRad, m_Translation_Z_Rot.Roll * degToRad));
		m_ZGhost->setWorldTransform(Transform);

		m_ArrowX->SetWorldRotation(m_Translation_X_Rot);
		m_ArrowY->SetWorldRotation(m_Translation_Y_Rot);
		m_ArrowZ->SetWorldRotation(m_Translation_Z_Rot);

		break;
	case SelectionGizmo::Rotation:

		//TODO: Rotate it based on the position from it
		Transform.setRotation(btQuaternion(m_Rotation_X_Rot.Yaw * degToRad, m_Rotation_X_Rot.Pitch * degToRad, m_Rotation_X_Rot.Roll * degToRad));
		m_XGhost->setWorldTransform(Transform);
		Transform.setRotation(btQuaternion(m_Rotation_Y_Rot.Yaw * degToRad, m_Rotation_Y_Rot.Pitch * degToRad, m_Rotation_Y_Rot.Roll * degToRad));
		m_YGhost->setWorldTransform(Transform);
		Transform.setRotation(btQuaternion(m_Rotation_Z_Rot.Yaw * degToRad, m_Rotation_Z_Rot.Pitch * degToRad, m_Rotation_Z_Rot.Roll * degToRad));
		m_ZGhost->setWorldTransform(Transform);

		m_ArrowX->SetWorldRotation(m_Rotation_X_Rot);
		m_ArrowY->SetWorldRotation(m_Rotation_Y_Rot);
		m_ArrowZ->SetWorldRotation(m_Rotation_Z_Rot);

		break;

		break;
	default:
		break;
	}


	btDiscreteDynamicsWorld* physicsWorld = World::GetPhysicsWorld();
	if (physicsWorld)
	{
		physicsWorld->updateSingleAabb(m_XGhost);
		physicsWorld->updateSingleAabb(m_YGhost);
		physicsWorld->updateSingleAabb(m_ZGhost);
	}
}

void SelectionGizmo::UpdateRotation(Rotator Rotation)
{
}

void SelectionGizmo::SetScale(Vector Scale)
{
	m_ArrowX->SetRelativeScale(Scale);
	m_ArrowY->SetRelativeScale(Scale);
	m_ArrowZ->SetRelativeScale(Scale);

	//Update this to the physics world
	btDiscreteDynamicsWorld* physicsWorld = World::GetPhysicsWorld();
	m_XCollision->setLocalScaling(Scale.ToBulletVector());
	m_YCollision->setLocalScaling(Scale.ToBulletVector());
	m_ZCollision->setLocalScaling(Scale.ToBulletVector());
	if (physicsWorld)
	{
		physicsWorld->updateSingleAabb(m_XGhost);
		physicsWorld->updateSingleAabb(m_YGhost);
		physicsWorld->updateSingleAabb(m_ZGhost);
	}

}

void SelectionGizmo::Render()
{
	if (!_Visible)
		return;

	Renderer::Submit(m_ArrowX);
	Renderer::Submit(m_ArrowY);
	Renderer::Submit(m_ArrowZ);
}

Vector SelectionGizmo::GetPosition() const
{
	btVector3 T = m_XGhost->getWorldTransform().getOrigin();
	return Vector(T.x(), T.y(), T.z());
}

void SelectionGizmo::SetVisibility(bool Visible)
{
	this->_Visible = Visible;
}

void SelectionGizmo::AddCollidersToWorld(World* World)
{
	World->AddCollisionObject(m_XGhost);
	World->AddCollisionObject(m_YGhost);
	World->AddCollisionObject(m_ZGhost);
}

void SelectionGizmo::RemoveCollidersFromWorld(World* World)
{
	World::GetPhysicsWorld()->removeCollisionObject(m_XGhost);
	World::GetPhysicsWorld()->removeCollisionObject(m_YGhost);
	World::GetPhysicsWorld()->removeCollisionObject(m_ZGhost);
}

void SelectionGizmo::Reset()
{
	m_SelectedAxis = Axis::None;
	m_SelectedComponent = nullptr;
	_Visible = false;
}

void SelectionGizmo::OnSelected(Axis Axis, WorldComponent* Object)
{
	m_SelectedAxis = Axis;
	m_SelectedComponent = Object;
	m_SelectedComponentStartPosition = Object->GetWorldPosition();
	m_SelectedComponentStartScale = Object->GetWorldScale();
	m_MouseLastUpdate = Input::GetMousePosition();

	Vector AxisDirection;
	switch (m_SelectedAxis)
	{
	case Axis::X:
		AxisDirection = Vector(1.0f, 0.0f, 0.0f);
		break;
	case Axis::Y:
		AxisDirection = Vector(0.0f, 1.0f, 0.0f);
		break;
	case Axis::Z:
		AxisDirection = Vector(0.0f, 0.0f, 1.0f);
		break;
	}



	Vector Start = RenderCommand::GetMainCamera()->GetCameraPosition();
	Vector Direction = RenderCommand::GetScreenToWorldDirectionVector(m_MouseLastUpdate.X, m_MouseLastUpdate.Y, EditorLayer::GetEditor()->GetSceneWindowSize(), EditorLayer::GetEditor()->GetSceneWindowPosition());

	float SelectionAxisClosestScale;
	float RayClosestScale;
	//Get the closest point on the selection gizmo axis to the mouse click
	Math::GetClosestDistanceBetweenLines(Ray(m_ArrowX->GetWorldPosition(), AxisDirection), Ray(Start, Direction), SelectionAxisClosestScale, RayClosestScale);
	m_ArrowOffset = AxisDirection * SelectionAxisClosestScale;
}

void SelectionGizmo::OnNewComponentSelected(WorldComponent* Object)
{
	m_SelectedComponent = Object;
}

void SelectionGizmo::OnDeselected()
{
	//Update the bounding box since it isnt done when we move the rigidbody
	//if(m_SelectedComponent)
	//	m_SelectedComponent->UpdateAABB();

	m_SelectedAxis = Axis::None;
}

StaticMeshComponent* SelectionGizmo::GetArrow(Axis axis) const
{
	switch (axis)
	{
	case Axis::X:
		return m_ArrowX;
	case Axis::Y:
		return m_ArrowY;
	case Axis::Z:
		return m_ArrowZ;
	default:
		FLOW_ENGINE_WARNING("SelectionGizmo::GetArrow: Case Error.");
		return nullptr;
	}

	return nullptr;
}

btCollisionShape* SelectionGizmo::GetArrowCollision(Axis axis) const
{
	switch (axis)
	{
	case Axis::X:
		return m_XCollision;
	case Axis::Y:
		return m_YCollision;
	case Axis::Z:
		return m_ZCollision;
	default:
		FLOW_ENGINE_WARNING("SelectionGizmo::GetArrowCollision: Case Error.");
		return nullptr;
	}

	return nullptr;
}

void SelectionGizmo::SetTransformationMode(SelectionGizmo::Transform newMode)
{
	m_TransformMode = newMode;

	switch (m_TransformMode)
	{
	case SelectionGizmo::Translation:
		m_ArrowX->SetStaticMesh(m_MeshTranslate);
		m_ArrowY->SetStaticMesh(m_MeshTranslate);
		m_ArrowZ->SetStaticMesh(m_MeshTranslate);
		break;
	case SelectionGizmo::Rotation:
		m_ArrowX->SetStaticMesh(m_MeshRotate);
		m_ArrowY->SetStaticMesh(m_MeshRotate);
		m_ArrowZ->SetStaticMesh(m_MeshRotate);
		break;
	case SelectionGizmo::Scale:
		m_ArrowX->SetStaticMesh(m_MeshScale);
		m_ArrowY->SetStaticMesh(m_MeshScale);
		m_ArrowZ->SetStaticMesh(m_MeshScale);
		break;
	default:
		break;
	}
}

void SelectionGizmo::GenerateCollisionData(StaticMeshComponent* Component, btCollisionShape*& Collider, btGhostObject*& Ghost)
{
	//Generate the collision
	btConvexHullShape* Shape = new btConvexHullShape();
	auto Vertices = Component->GetMesh()->GetVertices();
	for (auto Vert : Vertices)
	{
		btVector3 btv = btVector3(Vert._Position.X, Vert._Position.Y, Vert._Position.Z);
		Shape->addPoint(btv);
	}
	Collider = Shape;

	Collider->setLocalScaling(btVector3(1, 1, 1));


	//Create the ghost object
	btTransform Transform;
	Vector Location = Component->GetWorldPosition();
	Transform.setOrigin(btVector3(Location.X, Location.Y, Location.Z));
	Ghost->setCollisionShape(Collider);
	Ghost->setWorldTransform(Transform);
	Ghost->setUserPointer(Component);
}