#include "Flowpch.h"
#include "WorldComponent.h"
#include "Flow\GameFramework\Actor.h"
#include "ThirdParty\ImGui\imgui.h"
#include "Flow\Editor\Inspector.h"

#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"
#include "Flow/Physics/MotionState.h"

#include "Flow/GameFramework/World.h"

#include "Flow/GameFramework/Other/ClassFactory.h"

WorldComponent::WorldComponent()
	: WorldComponent("Unnamed WorldComponent")
{
}

WorldComponent::WorldComponent(const std::string& Name)
	: Component(Name), _ParentComponent(nullptr), _RigidBody(nullptr), m_CollisionShape(nullptr),
	_MotionState(nullptr), m_CollisionEnabled(true)
{
}

WorldComponent::~WorldComponent()
{
	_Children.clear();

	delete _RigidBody;
	delete m_CollisionShape;
	delete _MotionState;
}

#if WITH_EDITOR
void WorldComponent::EditorBeginPlay()
{
	for (auto& Child : _Children)
	{
		Child->EditorBeginPlay();
	}
}
void WorldComponent::OnViewportSelected()
{
}
void WorldComponent::OnViewportDeselected()
{
}
#endif

void WorldComponent::BeginPlay()
{
	for (auto& Child : _Children)
	{
		Child->BeginPlay();
	}
}

void WorldComponent::Tick(float DeltaTime)
{
	PROFILE_FUNCTION();

	for (auto& Child : _Children)
	{
		Child->Tick(DeltaTime);
	}
}

void WorldComponent::AddChild(WorldComponent* Child)
{
	CHECK_RETURN(!Child, "WorldComponent::AddChild: New Child was nullptr");
	_Children.push_back(Child);
	Child->SetParentComponent(this);
}

Vector3 WorldComponent::GetWorldPosition() const
{
	WorldComponent* Parent = GetParentComponent(); //TODO: Rotate this by the parents rotation
	//return Parent ? Parent->GetWorldPosition() + _RelativeTransform._Position : _RelativeTransform._Position;

	if (Parent)
	{
		Rotator ParentRotation = Rotator::AsRadians(Parent->GetWorldRotation());

		DirectX::XMVECTOR Position = DirectX::XMLoadFloat3(&static_cast<DirectX::XMFLOAT3>(_RelativeTransform.m_Position));
		DirectX::XMVECTOR Rotation = DirectX::XMQuaternionRotationRollPitchYaw(ParentRotation.Pitch, ParentRotation.Yaw, ParentRotation.Roll);
		DirectX::XMVECTOR NewPosition = DirectX::XMVector3Rotate(Position, Rotation);
		DirectX::XMFLOAT3 RotatedPos;
		DirectX::XMStoreFloat3(&RotatedPos, NewPosition);

		return Parent->GetWorldPosition() + Vector3(RotatedPos.x, RotatedPos.y, RotatedPos.z);
	}
	else
		return _RelativeTransform.m_Position;
}

Vector3 WorldComponent::GetRelativePosition() const
{
	return _RelativeTransform.m_Position;
}

void WorldComponent::SetWorldPosition(Vector3 NewPosition)
{
	Vector3 CurrentParentWorld;
	WorldComponent* Parent = GetParentComponent();
	while (Parent)
	{
		CurrentParentWorld += Parent->_RelativeTransform.m_Position;
		Parent = Parent->GetParentComponent();
	}

	_RelativeTransform.m_Position = NewPosition - CurrentParentWorld;

	// Update the physics transform. Data isnt grabbed from the motion state at runtime unless object is kinematic
	if (_RigidBody)
	{
		btTransform NewTransform;
		//We assume that the object is the root component
		NewTransform.setOrigin(btVector3(_RelativeTransform.m_Position));

		//Convert to radians for now, need to do rotations better lul
		btQuaternion Rotation;
		Rotator RadiansRotation = Rotator::AsRadians(_RelativeTransform.m_Rotation);
		Rotation.setEuler(RadiansRotation.Yaw, RadiansRotation.Pitch, RadiansRotation.Roll);
		NewTransform.setRotation(Rotation);


		_RigidBody->setWorldTransform(NewTransform);
		//_RigidBody->clearForces();
		//_RigidBody->setLinearVelocity(btVector3(0.0f, 0.0f, 0.0f));
		_RigidBody->activate();
	}

}

void WorldComponent::SetRelativePosition(Vector3 NewPosition)
{
	_RelativeTransform.m_Position = NewPosition;
}

void WorldComponent::AddRelativePosition(Vector3 Position)
{
	_RelativeTransform.m_Position += Position;
}

Rotator WorldComponent::GetWorldRotation() const
{
	WorldComponent* Parent = GetParentComponent();
	return Parent ? Parent->GetWorldRotation() + _RelativeTransform.m_Rotation : _RelativeTransform.m_Rotation;
}

Rotator WorldComponent::GetRelativeRotation() const
{
	return _RelativeTransform.m_Rotation;
}

void WorldComponent::SetWorldRotation(Rotator NewRotation)
{
	Rotator CurrentParentWorld;

	if (WorldComponent* Parent = GetParentComponent())
		CurrentParentWorld = Parent->GetWorldRotation();

	_RelativeTransform.m_Rotation = NewRotation - CurrentParentWorld;
	//TODO: UpdatePhysics Movement
}

void WorldComponent::SetRelativeRotation(Rotator NewRotation)
{
	_RelativeTransform.m_Rotation = NewRotation;
	//TODO: UpdatePhysics Movement
}

void WorldComponent::AddRelativeRotation(Rotator Rotation)
{
	_RelativeTransform.m_Rotation += Rotation;
}

Vector3 WorldComponent::GetWorldScale() const
{
	WorldComponent* Parent = GetParentComponent();
	return Parent ? Parent->GetWorldScale() * _RelativeTransform.m_Scale : _RelativeTransform.m_Scale;
}

Vector3 WorldComponent::GetRelativeScale() const
{
	return _RelativeTransform.m_Scale;
}

void WorldComponent::SetWorldScale(Vector3 NewScale)
{
	Vector3 CurrentParentWorld;
	WorldComponent* PointedComp = GetParentComponent();
	while (PointedComp)
	{
		CurrentParentWorld *= PointedComp->_RelativeTransform.m_Scale;
		PointedComp = PointedComp->GetParentComponent();
	}

	_RelativeTransform.m_Scale = NewScale - CurrentParentWorld;

	UpdateCollisionScale();
}

void WorldComponent::SetRelativeScale(Vector3 NewScale)
{
	_RelativeTransform.m_Scale = NewScale;

	UpdateCollisionScale();
}

Transform WorldComponent::GetWorldTransform() const
{
	return Transform(GetWorldPosition(), GetWorldRotation(), GetWorldScale());
}

Transform WorldComponent::GetRelativeTransform() const
{
	return _RelativeTransform;
}

void WorldComponent::SetWorldTransform(Transform NewTransform)
{
	SetWorldPosition(NewTransform.m_Position);
	SetWorldRotation(NewTransform.m_Rotation);
	SetWorldScale(NewTransform.m_Scale);
}

void WorldComponent::SetRelativeTransform(Transform NewTransform)
{
	_RelativeTransform = NewTransform;
}

void WorldComponent::Render()
{
	PROFILE_CURRENT_SCOPE("Render Children");

	for (auto Child : _Children)
	{
		if (Child)
			Child->Render();
	}
}

Vector3* WorldComponent::GetWriteablePosition()
{
	return &_RelativeTransform.m_Position;
}
Rotator* WorldComponent::GetWriteableRotation()
{
	return &_RelativeTransform.m_Rotation;
}
Vector3* WorldComponent::GetWriteableScale()
{
	return &_RelativeTransform.m_Scale;
}
void WorldComponent::DrawInspectionTree(WorldComponent* CurrentInspectedComponent, bool DontOpenTree)
{
	const ImVec2 ButtonSize = ImVec2(100, 12);
	const float RightOffset = 20;

	if (DontOpenTree)
		ImGui::SetNextItemOpen(false);

	bool NodeOpen = ImGui::TreeNode(GetName().c_str());

	if (ImGui::IsItemClicked())
		Inspector::UpdateSelectedComponent(this);

	if (!DontOpenTree && CurrentInspectedComponent == this)
	{
		ImGui::SameLine();
		ImGui::TextColored(IMGUI_GREEN, "Selected");
	}

	if (NodeOpen)
	{
		ImVec2 pos = ImGui::GetCursorScreenPos();
		ImU32 col = ImColor(ImGui::GetStyle().Colors[ImGuiCol_HeaderHovered]);

		for (auto& Child : _Children)
		{
			Child->DrawInspectionTree(CurrentInspectedComponent, DontOpenTree);
		}
		ImGui::TreePop();
	}

}
void WorldComponent::DrawDetailsWindow(bool bDontUpdate)
{
}
std::vector<WorldComponent*> WorldComponent::GetChildren() const
{
	return _Children;
}

WorldComponent* WorldComponent::GetChildByName(const std::string& ChildName)
{
	for (auto& Comp : _Children)
	{
		if (Comp->GetName() == ChildName)
			return Comp;
	}

	return nullptr;
}

void WorldComponent::SetVisibility(bool Visible)
{
	_Visible = Visible;
}
bool WorldComponent::IsVisible() const
{
	return _Visible;
}

void WorldComponent::InitialisePhysics()
{
	_SimulatePhysics = false;
}

void WorldComponent::DestroyPhysics()
{
}

void WorldComponent::CreateRigidBody()
{
	if (!m_CollisionEnabled)
	{
		return;
	}

	//Convert transform into BT Quaternion
	btQuaternion Rotation;
	Rotator Rot = Rotator::AsRadians(GetWorldRotation());
	Rotation.setEulerZYX(Rot.Roll, Rot.Yaw, Rot.Pitch);

	//FOR NOW WE ASSUME THAT THE PHYSICS COMPONENT IS THE ROOT COMPONENT
	MotionState* motionState = new MotionState(&_RelativeTransform.m_Position, &_RelativeTransform.m_Rotation);

	//Initialise the mass and intertia values
	btScalar bodyMass = _SimulatePhysics ? 20.0f : 0.0f;
	btVector3 bodyInertia;
	m_CollisionShape->calculateLocalInertia(bodyMass, bodyInertia);

	//Create the construction info for the body
	btRigidBody::btRigidBodyConstructionInfo bodyCI =
		btRigidBody::btRigidBodyConstructionInfo(bodyMass, motionState, m_CollisionShape, bodyInertia);
	bodyCI.m_restitution = 0.4f;
	bodyCI.m_friction = 0.5f;
	bodyCI.m_rollingFriction = 0.2f;
	bodyCI.m_spinningFriction = 0.3f;

	_RigidBody = new btRigidBody(bodyCI);
	_RigidBody->setUserPointer(this);

	//TODO: Testing flag issues
	_RigidBody->setCollisionFlags(0);
}

void WorldComponent::UpdateAABB()
{
	if(_RigidBody)
		World::GetPhysicsWorld()->updateSingleAabb(_RigidBody);
}

btRigidBody* WorldComponent::GetRigidBody() const
{
	return _RigidBody;
}

btCollisionShape* WorldComponent::GetCollisionShape() const
{
	return m_CollisionShape;
}

void WorldComponent::SetSimulatePhysics(bool Simulate)
{
	_SimulatePhysics = Simulate;
}

bool WorldComponent::IsSimulatingPhysics() const
{
	return _SimulatePhysics;
}

bool WorldComponent::HasCollision() const
{
	return m_CollisionShape;
}

void WorldComponent::UpdateCollisionScale()
{
	if (!m_CollisionEnabled || !m_CollisionShape || !_RigidBody)
	{
		return;
	}

	m_CollisionShape->setLocalScaling(_RelativeTransform.m_Scale);
	World::GetPhysicsWorld()->updateSingleAabb(_RigidBody);
}

std::string WorldComponent::GetClassSerializationUID(std::ofstream* Archive)
{
	return typeid(WorldComponent).name();
}

void WorldComponent::Serialize(std::ofstream* Archive)
{
	//= Object Class ======
	std::string ClassUID = GetClassSerializationUID(Archive);
	Archive->write(ClassUID.c_str(), sizeof(char) * 32);

	// Component Name (TODO: Decide on a fixed string size)
	Archive->write(GetName().c_str(), sizeof(char) * 32);

	// Name of parent component
	auto Parent = GetParentComponent();
	(Parent) ?	Archive->write(Parent->GetName().c_str(), sizeof(char) * 32) :	Archive->write("None", sizeof(char) * 32);
	
	// Write the component transform
	Archive->write(reinterpret_cast<char*>(&GetRelativeTransform()), sizeof(Transform));

	//= Physics ===
	Archive->write(reinterpret_cast<char*>(&_SimulatePhysics), sizeof(bool));
}

void WorldComponent::SerializeChildren(std::ofstream* Archive)
{
	//Write all child components
	for (auto& Comp : GetChildren())
	{
		Comp->Serialize(Archive);
		Comp->SerializeChildren(Archive);
	}
}

void WorldComponent::Deserialize(std::ifstream* Archive, Actor* NewParent)
{
	// Component Name
	char ComponentName[32] = "";
	Archive->read(ComponentName, sizeof(char) * 32);
	SetName(ComponentName);

	// Set the parent Actor
	SetParent(NewParent);

	// Load Component Parent
	Archive->read(ComponentName, sizeof(char) * 32);
	std::string CompNameStr(ComponentName);
	if (CompNameStr != "None")
	{
		auto Comp = NewParent->GetComponentByName(ComponentName);

		Comp ?
			SetParentComponent(dynamic_cast<WorldComponent*>(Comp)) :
			FLOW_ENGINE_ERROR("StaticMeshComponent::Deserialize: Failed to load component parent with name {0}", ComponentName);
	}

	//Load Component Transform
	Transform NewTrans;
	Archive->read(reinterpret_cast<char*>(&NewTrans), sizeof(Transform));
	SetRelativeTransform(NewTrans);

	//= Load Physics properties
	Archive->read(reinterpret_cast<char*>(&_SimulatePhysics), sizeof(bool));
}

void WorldComponent::DeserializeChildren(std::ifstream* Archive, Actor* NewParent)
{
	for (auto& Comp : _Children)
	{
		Comp->Deserialize(Archive, NewParent);
		Comp->DeserializeChildren(Archive, NewParent);
	}
}


