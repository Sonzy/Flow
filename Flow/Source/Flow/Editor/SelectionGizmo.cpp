#include "Flowpch.h"
#include "SelectionGizmo.h"

#include "Flow\Rendering\Renderer.h"
#include "Flow\Assets\Meshes\MeshAsset.h"
#include "Flow\GameFramework\Components\StaticMeshComponent.h"

#include "Flow/GameFramework/World.h"
#include "Flow\Assets\AssetSystem.h"

#include "Bullet/btBulletCollisionCommon.h"
#include "Bullet/btBulletDynamicsCommon.h"
#include "Bullet/BulletCollision\CollisionDispatch\btGhostObject.h"

#include "Flow/Input/Input.h"

#include "Maths/Vector3.h"

#include "Flow/Editor/Editor.h"

#include "Physics/Physics.h"

SelectionGizmo::SelectionGizmo()
	: Actor("SelectionGizmo") //TODO: are we keeping this an actor?
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

	m_ArrowX->SetStaticMesh(m_MeshTranslate);
	m_ArrowY->SetStaticMesh(m_MeshTranslate);
	m_ArrowZ->SetStaticMesh(m_MeshTranslate);

	m_ArrowX->m_Tag = "ArrowX";
	m_ArrowY->m_Tag = "ArrowY";
	m_ArrowZ->m_Tag = "ArrowZ";

	m_RootComponent = m_Root;
	m_RootComponent->AddChild(m_ArrowX);
	m_RootComponent->AddChild(m_ArrowY);
	m_RootComponent->AddChild(m_ArrowZ);

	m_ArrowX->SetParent(this);
	m_ArrowY->SetParent(this);
	m_ArrowZ->SetParent(this);

	m_ArrowX->SetWorldRotation(m_Translation_X_Rot);
	m_ArrowY->SetWorldRotation(m_Translation_Y_Rot);
	m_ArrowZ->SetWorldRotation(m_Translation_Z_Rot);

	const float Scale = 1.0f;
	m_ArrowX->SetWorldScale(Scale);
	m_ArrowY->SetWorldScale(Scale);
	m_ArrowZ->SetWorldScale(Scale);

	m_ArrowX->SetStencilMode(StencilMode::AlwaysOnTop);
	m_ArrowY->SetStencilMode(StencilMode::AlwaysOnTop);
	m_ArrowZ->SetStencilMode(StencilMode::AlwaysOnTop);

	m_ArrowX->m_DrawWithoutDepth = true;
	m_ArrowY->m_DrawWithoutDepth = true;
	m_ArrowZ->m_DrawWithoutDepth = true;

	m_XGhost = new btGhostObject();
	m_YGhost = new btGhostObject();
	m_ZGhost = new btGhostObject();

	GenerateCollision();

	//Initialise the meshes
	m_ArrowX->RefreshBinds();
	m_ArrowY->RefreshBinds();
	m_ArrowZ->RefreshBinds();

	m_TransformMode = Transform::Translation;
	m_Visible = false;
}

SelectionGizmo::~SelectionGizmo()
{
	delete m_XGhost;
	delete m_YGhost;
	delete m_ZGhost;

	delete m_ArrowX;
	delete m_ArrowY;
	delete m_ArrowZ;
}

void SelectionGizmo::InitialisePhysics()
{

}

void SelectionGizmo::UpdateSelection()
{
	if (m_SelectedAxis == Axis::None)
	{
		if (m_SelectedComponent)
			UpdatePosition(m_SelectedComponent->GetWorldPosition());
			
		return;
	}

	IntVector2 MousePosition = Input::GetMousePosition();
	IntVector2 MouseDifference = MousePosition - m_MouseLastUpdate;
	bool Add = MouseDifference.x > 0;

	if (MousePosition.Distance(m_MouseLastUpdate) < m_MouseDistanceThreshold)
		return;

	Vector3 AxisDirection;
	switch (m_SelectedAxis)
	{
	case Axis::X:
		AxisDirection = Vector3(1.0f, 0.0f, 0.0f);
		break;
	case Axis::Y:
		AxisDirection = Vector3(0.0f, 1.0f, 0.0f);
		break;
	case Axis::Z:
		AxisDirection = Vector3(0.0f, 0.0f, 1.0f);
		break;
	}

	//Fire a ray from the current mouse position.
	Vector3 Start = RenderCommand::GetMainCamera()->GetCameraPosition();
	Vector3 Direction = RenderCommand::GetScreenToWorldDirectionVector(MousePosition.x, MousePosition.y, Editor::Get().GetSceneWindowSize(), Editor::Get().GetSceneWindowPosition());

	switch (m_TransformMode)
	{
	case SelectionGizmo::Translation:
	{
		//Get the closest point on the axis line to the ray
		float AxisClosestScale;
		float RayClosestScale;
		Maths::GetClosestDistanceBetweenLines(Physics::Ray(m_SelectedComponentStartPosition /*TODO: + the distance in axis where mouse originally clicked */, AxisDirection), Physics::Ray(Start, Direction), AxisClosestScale, RayClosestScale);

		Vector3 NewPosition = m_SelectedComponentStartPosition + (-AxisDirection * AxisClosestScale) + m_ArrowOffset;

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
		Maths::GetClosestDistanceBetweenLines(Physics::Ray(m_SelectedComponentStartPosition /*TODO: + the distance in axis where mouse originally clicked */, AxisDirection), Physics::Ray(Start, Direction), AxisClosestScale, RayClosestScale);

		Vector3 NewPosition = m_SelectedComponentStartPosition + (-AxisDirection * AxisClosestScale) + m_ArrowOffset;

		//TODO:  Update
		const float scaleSensitivity = 1.0f;
		float diffInAxis = 0;
		switch (m_SelectedAxis)
		{
		case Axis::X: diffInAxis = NewPosition.x - m_SelectedComponentStartPosition.x;	break;
		case Axis::Y: diffInAxis = NewPosition.y - m_SelectedComponentStartPosition.y;	break;
		case Axis::Z: diffInAxis = NewPosition.z - m_SelectedComponentStartPosition.z;	break;
		}

		Vector3 prevScale = m_SelectedComponent->GetWorldScale();
		switch (m_SelectedAxis)
		{
		case Axis::X: m_SelectedComponent->SetWorldScale(Vector3(m_SelectedComponentStartScale.x + (diffInAxis * scaleSensitivity), prevScale.y, prevScale.z));	break;
		case Axis::Y: m_SelectedComponent->SetWorldScale(Vector3(prevScale.x, m_SelectedComponentStartScale.y + (diffInAxis * scaleSensitivity), prevScale.z));	break;
		case Axis::Z: m_SelectedComponent->SetWorldScale(Vector3(prevScale.x, prevScale.y, m_SelectedComponentStartScale.z + (diffInAxis * scaleSensitivity)));	break;
		}

		m_MouseLastUpdate = MousePosition;
		break;
	}
	}
}

void SelectionGizmo::UpdatePosition(Vector3 Position)
{
	m_ArrowX->SetWorldPosition(Position);
	m_ArrowY->SetWorldPosition(Position);
	m_ArrowZ->SetWorldPosition(Position);

	float degToRad = (Maths::PI / 180.0f);

	btTransform Transform;
	Transform.setOrigin(btVector3(Position.x, Position.y, Position.z));

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

void SelectionGizmo::SetScale(Vector3 Scale)
{
	m_ArrowX->SetRelativeScale(Scale);
	m_ArrowY->SetRelativeScale(Scale);
	m_ArrowZ->SetRelativeScale(Scale);

	//Update this to the physics world
	btDiscreteDynamicsWorld* physicsWorld = World::GetPhysicsWorld();

	//Scale all of them so we dont have to bother if we change
	m_CollisionTranslation.setLocalScaling(Scale);
	m_CollisionRotation.setLocalScaling(Scale);
	m_CollisionScale.setLocalScaling(Scale);

	if (physicsWorld)
	{
		physicsWorld->updateSingleAabb(m_XGhost);
		physicsWorld->updateSingleAabb(m_YGhost);
		physicsWorld->updateSingleAabb(m_ZGhost);
	}

}

void SelectionGizmo::Render()
{
	if (!m_Visible)
		return;

	Renderer::Submit(m_ArrowX);
	Renderer::Submit(m_ArrowY);
	Renderer::Submit(m_ArrowZ);
}

Vector3 SelectionGizmo::GetPosition() const
{
	//btVector3 T = m_XGhost->getWorldTransform().getOrigin();
	//return Vector3(T.x(), T.y(), T.z());

	return m_XGhost->getWorldTransform().getOrigin();
}

void SelectionGizmo::SetVisibility(bool Visible)
{
	this->m_Visible = Visible;
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
	m_Visible = false;
}

void SelectionGizmo::OnSelected(Axis Axis, WorldComponent* Object)
{
	m_SelectedAxis = Axis;
	m_SelectedComponent = Object;
	m_SelectedComponentStartPosition = Object->GetWorldPosition();
	m_SelectedComponentStartScale = Object->GetWorldScale();
	m_MouseLastUpdate = Input::GetMousePosition();

	Vector3 AxisDirection;
	switch (m_SelectedAxis)
	{
	case Axis::X:
		AxisDirection = Vector3(1.0f, 0.0f, 0.0f);
		break;
	case Axis::Y:
		AxisDirection = Vector3(0.0f, 1.0f, 0.0f);
		break;
	case Axis::Z:
		AxisDirection = Vector3(0.0f, 0.0f, 1.0f);
		break;
	}



	Vector3 Start = RenderCommand::GetMainCamera()->GetCameraPosition();
	Vector3 Direction = RenderCommand::GetScreenToWorldDirectionVector(m_MouseLastUpdate.x, m_MouseLastUpdate.y, Editor::Get().GetSceneWindowSize(), Editor::Get().GetSceneWindowPosition());

	float SelectionAxisClosestScale;
	float RayClosestScale;
	//Get the closest point on the selection gizmo axis to the mouse click
	Maths::GetClosestDistanceBetweenLines(Physics::Ray(m_ArrowX->GetWorldPosition(), AxisDirection), Physics::Ray(Start, Direction), SelectionAxisClosestScale, RayClosestScale);
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

void SelectionGizmo::SetTransformationMode(SelectionGizmo::Transform newMode)
{
	if (!m_Visible)
	{
		return;
	}

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
	}

	auto* physWorld = World::GetPhysicsWorld();
	physWorld->removeCollisionObject(m_XGhost);
	physWorld->removeCollisionObject(m_YGhost);
	physWorld->removeCollisionObject(m_ZGhost);

	switch (m_TransformMode)
	{
	case SelectionGizmo::Translation:
		m_XGhost->setCollisionShape(&m_CollisionTranslation);
		m_YGhost->setCollisionShape(&m_CollisionTranslation);
		m_ZGhost->setCollisionShape(&m_CollisionTranslation);
		break;
	case SelectionGizmo::Rotation:
		m_XGhost->setCollisionShape(&m_CollisionRotation);
		m_YGhost->setCollisionShape(&m_CollisionRotation);
		m_ZGhost->setCollisionShape(&m_CollisionRotation);
		break;
	case SelectionGizmo::Scale:
		m_XGhost->setCollisionShape(&m_CollisionScale);
		m_YGhost->setCollisionShape(&m_CollisionScale);
		m_ZGhost->setCollisionShape(&m_CollisionScale);
		break;
	}

	physWorld->addCollisionObject(m_XGhost);
	physWorld->addCollisionObject(m_YGhost);
	physWorld->addCollisionObject(m_ZGhost);

}

Axis SelectionGizmo::TagToAxis(const std::string& Tag)
{
	if (Tag._Equal("ArrowX"))
	{
		return Axis::X;
	}
	else if (Tag._Equal("ArrowY"))
	{
		return Axis::Y;
	}
	else if (Tag._Equal("ArrowZ"))
	{
		return Axis::Z;
	}

	return Axis::None;
}

void SelectionGizmo::GenerateCollision()
{
	//Generate the collision
	const std::vector<Vector3>& TranslationVerts = m_MeshTranslate->GetMesh(0)->GetCollisionVertices();
	const std::vector<Vector3>& RotationVerts = m_MeshRotate->GetMesh(0)->GetCollisionVertices();
	const std::vector<Vector3>& ScaleVerts = m_MeshScale->GetMesh(0)->GetCollisionVertices();

	for (const Vector3& Vert : TranslationVerts)
	{
		btVector3 btv = btVector3(Vert.x, Vert.y, Vert.z);
		m_CollisionTranslation.addPoint(btv);
	}

	for (const Vector3& Vert : RotationVerts)
	{
		btVector3 btv = btVector3(Vert.x, Vert.y, Vert.z);
		m_CollisionRotation.addPoint(btv);
	}

	for (const Vector3& Vert : ScaleVerts)
	{
		btVector3 btv = btVector3(Vert.x, Vert.y, Vert.z);
		m_CollisionScale.addPoint(btv);
	}

	m_CollisionTranslation.setLocalScaling(btVector3(1, 1, 1));
	m_CollisionRotation.setLocalScaling(btVector3(1, 1, 1));
	m_CollisionScale.setLocalScaling(btVector3(1, 1, 1));


	//Create the ghost object
	btTransform Transform;
	Vector3 Location;

	Location = m_ArrowX->GetWorldPosition();
	Transform.setOrigin(btVector3(Location.x, Location.y, Location.z));
	m_XGhost->setCollisionShape(&m_CollisionTranslation);
	m_XGhost->setWorldTransform(Transform);
	m_XGhost->setUserPointer(m_ArrowX);

	Location = m_ArrowY->GetWorldPosition();
	Transform.setOrigin(btVector3(Location.x, Location.y, Location.z));
	m_YGhost->setCollisionShape(&m_CollisionTranslation);
	m_YGhost->setWorldTransform(Transform);
	m_YGhost->setUserPointer(m_ArrowY);

	Location = m_ArrowZ->GetWorldPosition();
	Transform.setOrigin(btVector3(Location.x, Location.y, Location.z));
	m_ZGhost->setCollisionShape(&m_CollisionTranslation);
	m_ZGhost->setWorldTransform(Transform);
	m_ZGhost->setUserPointer(m_ArrowZ);


	m_XGhost->setCollisionFlags(btGhostObject::CF_NO_CONTACT_RESPONSE);
	m_YGhost->setCollisionFlags(btGhostObject::CF_NO_CONTACT_RESPONSE);
	m_ZGhost->setCollisionFlags(btGhostObject::CF_NO_CONTACT_RESPONSE);
}