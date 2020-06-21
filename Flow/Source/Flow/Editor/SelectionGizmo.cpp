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

#include "Flow/Layers/EditorLayer.h"

SelectionGizmo::SelectionGizmo()
	: Actor("SelectionGizmo")
{
	MeshAsset* Meshes = AssetSystem::GetAsset<MeshAsset>("SelectionGizmo");
	Material* Mat = AssetSystem::GetAsset<MaterialAsset>("Mat_FlatColour_White")->GetMaterial();

	//Create and assign mesh components.
	_ArrowX = new StaticMeshComponent("_ArrowX", "SelectionGizmo", "Mat_FlatColour_White");
	_ArrowY = new StaticMeshComponent("_ArrowY", "SelectionGizmo", "Mat_FlatColour_White");
	_ArrowZ = new StaticMeshComponent("_ArrowZ", "SelectionGizmo", "Mat_FlatColour_White");

	_ArrowX->_Tag = "ArrowX";
	_ArrowY->_Tag = "ArrowY";
	_ArrowZ->_Tag = "ArrowZ";

	_ArrowX->SetParentComponent(nullptr);
	_RootComponent = _ArrowX;
	_RootComponent->AddChild(_ArrowY);
	_RootComponent->AddChild(_ArrowZ);

	_ArrowX->SetParent(this);
	_ArrowY->SetParent(this);
	_ArrowZ->SetParent(this);

	_ArrowX->SetWorldRotation(Rotator(0.0f, 90.0f, 0.0f));
	_ArrowY->SetWorldRotation(Rotator(0.0f, 0.0f, 0.0f));
	_ArrowZ->SetWorldRotation(Rotator(90.0f, 0.0f, 0.0f));

	//Init the transform
	const int Scale = 1.0f;
	_ArrowX->SetWorldScale(Scale);
	_ArrowY->SetWorldScale(Scale);
	_ArrowZ->SetWorldScale(Scale);

	//Initialise the meshes
	_ArrowX->RefreshBinds();
	_ArrowY->RefreshBinds();
	_ArrowZ->RefreshBinds();

	_ArrowX->SetStencilMode(StencilMode::AlwaysOnTop);
	_ArrowY->SetStencilMode(StencilMode::AlwaysOnTop);
	_ArrowZ->SetStencilMode(StencilMode::AlwaysOnTop);
}

SelectionGizmo::~SelectionGizmo()
{
	delete _XGhost;
	delete _YGhost;
	delete _ZGhost;

	delete _ArrowX;
	delete _ArrowY;
	delete _ArrowZ;

	delete _XCollision;
	delete _YCollision;
	delete _ZCollision;
}


void SelectionGizmo::GenerateCollision()
{
	_XGhost = new btGhostObject();
	_YGhost = new btGhostObject();
	_ZGhost = new btGhostObject();

	//Create collision data for each arrow seperately
	GenerateCollisionData(_ArrowX, _XCollision, _XGhost);
	GenerateCollisionData(_ArrowY, _YCollision, _YGhost);
	GenerateCollisionData(_ArrowZ, _ZCollision, _ZGhost);

	_XGhost->setCollisionFlags(btGhostObject::CF_NO_CONTACT_RESPONSE);
	_YGhost->setCollisionFlags(btGhostObject::CF_NO_CONTACT_RESPONSE);
	_ZGhost->setCollisionFlags(btGhostObject::CF_NO_CONTACT_RESPONSE);
}

void SelectionGizmo::InitialisePhysics()
{
	//TODO: Remove
	//_ArrowX->InitialisePhysics();
	//_ArrowY->InitialisePhysics();
	//_ArrowZ->InitialisePhysics();
}

void SelectionGizmo::UpdateSelection()
{
	if (_SelectedAxis == SelectedAxis::None)
	{
		if (_SelectedComponent)
			UpdatePosition(_SelectedComponent->GetWorldPosition());
			
		return;
	}

	IntVector2D MousePosition = Input::GetMousePosition();
	IntVector2D MouseDifference = MousePosition - _MouseLastUpdate;
	bool Add = MouseDifference.X > 0;

	if (MousePosition.Distance(_MouseLastUpdate) < _MouseDistanceThreshold)
		return;

	Vector AxisDirection;
	switch (_SelectedAxis)
	{
	case SelectedAxis::X:
		AxisDirection = Vector(1.0f, 0.0f, 0.0f);
		break;
	case SelectedAxis::Y:
		AxisDirection = Vector(0.0f, 1.0f, 0.0f);
		break;
	case SelectedAxis::Z:
		AxisDirection = Vector(0.0f, 0.0f, 1.0f);
		break;
	}

	//Fire a ray from the current mouse position.
	Vector Start = RenderCommand::GetMainCamera()->GetCameraPosition();
	Vector Direction = RenderCommand::GetScreenToWorldDirectionVector(MousePosition.X, MousePosition.Y, EditorLayer::GetEditor()->GetSceneWindowSize(), EditorLayer::GetEditor()->GetSceneWindowPosition());


	//Get the closest point on the axis line to the ray
	float AxisClosestScale;
	float RayClosestScale;
	Math::GetClosestDistanceBetweenLines(Ray(_SelectedComponentStartPosition /*TODO: + the distance in axis where mouse originally clicked */, AxisDirection), Ray(Start, Direction), AxisClosestScale, RayClosestScale);

	Vector NewPosition = _SelectedComponentStartPosition + (-AxisDirection * AxisClosestScale) + ArrowOffset;

	//Original Position + Closest point on x axis to mouse + Offset from object to arrow
	_SelectedComponent->SetWorldPosition(NewPosition);
	UpdatePosition(NewPosition);

	_MouseLastUpdate = MousePosition;

	//TODO: Make the collision were moving kinematic - or disable movement on non-kinematic object - or dont allow deactivation whilst moving
	// Update the bounding box of selected component whilst moving it
	if (_SelectedComponent)
		_SelectedComponent->UpdateAABB();
}

void SelectionGizmo::UpdatePosition(Vector Position)
{
	_ArrowX->SetWorldPosition(Position);
	_ArrowY->SetWorldPosition(Position);
	_ArrowZ->SetWorldPosition(Position);

	btTransform Transform;
	Transform.setOrigin(btVector3(Position.X, Position.Y, Position.Z));
	Transform.setRotation(btQuaternion(0.0f, 0.0f, Math::PI / 2));
	_XGhost->setWorldTransform(Transform);
	Transform.setRotation(btQuaternion(0.0f, 0.0f, 0.0f));
	_YGhost->setWorldTransform(Transform);
	Transform.setRotation(btQuaternion(0.0f, Math::PI / 2, 0.0f));
	_ZGhost->setWorldTransform(Transform);

	World::GetPhysicsWorld()->updateSingleAabb(_XGhost);
	World::GetPhysicsWorld()->updateSingleAabb(_YGhost);
	World::GetPhysicsWorld()->updateSingleAabb(_ZGhost);
}

void SelectionGizmo::UpdateRotation(Rotator Rotation)
{
}

void SelectionGizmo::SetScale(Vector Scale)
{
	_ArrowX->SetRelativeScale(Scale);
	_ArrowY->SetRelativeScale(Scale);
	_ArrowZ->SetRelativeScale(Scale);
}

void SelectionGizmo::Render()
{
	if (!_Visible)
		return;

	Renderer::Submit(_ArrowX);
	Renderer::Submit(_ArrowY);
	Renderer::Submit(_ArrowZ);
}

Vector SelectionGizmo::GetPosition() const
{
	btVector3 T = _XGhost->getWorldTransform().getOrigin();
	return Vector(T.x(), T.y(), T.z());
}

void SelectionGizmo::SetVisibility(bool Visible)
{
	this->_Visible = Visible;
}

void SelectionGizmo::AddCollidersToWorld(World* World)
{
	World->AddCollisionObject(_XGhost);
	World->AddCollisionObject(_YGhost);
	World->AddCollisionObject(_ZGhost);
}

void SelectionGizmo::RemoveCollidersFromWorld(World* World)
{
	World::GetPhysicsWorld()->removeCollisionObject(_XGhost);
	World::GetPhysicsWorld()->removeCollisionObject(_YGhost);
	World::GetPhysicsWorld()->removeCollisionObject(_ZGhost);
}

void SelectionGizmo::Reset()
{
	_SelectedAxis = SelectedAxis::None;
	_SelectedComponent = nullptr;
	_Visible = false;
}

void SelectionGizmo::OnSelected(SelectedAxis SelectedAxis, WorldComponent* Object)
{
	_SelectedAxis = SelectedAxis;
	_SelectedComponent = Object;
	_SelectedComponentStartPosition = Object->GetWorldPosition();
	_MouseLastUpdate = Input::GetMousePosition();

	Vector AxisDirection;
	switch (_SelectedAxis)
	{
	case SelectedAxis::X:
		AxisDirection = Vector(1.0f, 0.0f, 0.0f);
		break;
	case SelectedAxis::Y:
		AxisDirection = Vector(0.0f, 1.0f, 0.0f);
		break;
	case SelectedAxis::Z:
		AxisDirection = Vector(0.0f, 0.0f, 1.0f);
		break;
	}



	Vector Start = RenderCommand::GetMainCamera()->GetCameraPosition();
	Vector Direction = RenderCommand::GetScreenToWorldDirectionVector(_MouseLastUpdate.X, _MouseLastUpdate.Y, EditorLayer::GetEditor()->GetSceneWindowSize(), EditorLayer::GetEditor()->GetSceneWindowPosition());

	float SelectionAxisClosestScale;
	float RayClosestScale;
	//Get the closest point on the selection gizmo axis to the mouse click
	Math::GetClosestDistanceBetweenLines(Ray(_ArrowX->GetWorldPosition(), AxisDirection), Ray(Start, Direction), SelectionAxisClosestScale, RayClosestScale);
	ArrowOffset = AxisDirection * SelectionAxisClosestScale;
}

void SelectionGizmo::OnNewComponentSelected(WorldComponent* Object)
{
	_SelectedComponent = Object;
}

void SelectionGizmo::OnDeselected()
{
	//Update the bounding box since it isnt done when we move the rigidbody
	//if(_SelectedComponent)
	//	_SelectedComponent->UpdateAABB();

	_SelectedAxis = SelectedAxis::None;
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