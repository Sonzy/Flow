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
	: Component(Name), _ParentComponent(nullptr), _RigidBody(nullptr), _CollisionShape(nullptr),
	_MotionState(nullptr)
{
	ClassFactory::Get().RegisterFactoryClass<WorldComponent>();
}

WorldComponent::~WorldComponent()
{
	_Children.clear();

	delete _RigidBody;
	delete _CollisionShape;
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

Vector WorldComponent::GetWorldPosition() const
{
	WorldComponent* Parent = GetParentComponent(); //TODO: Rotate this by the parents rotation
	//return Parent ? Parent->GetWorldPosition() + _RelativeTransform._Position : _RelativeTransform._Position;

	if (Parent)
	{
		Rotator ParentRotation = Rotator::AsRadians(Parent->GetWorldRotation());

		DirectX::XMVECTOR Position = DirectX::XMLoadFloat3(&_RelativeTransform._Position.ToDXFloat3());
		DirectX::XMVECTOR Rotation = DirectX::XMQuaternionRotationRollPitchYaw(ParentRotation.Pitch, ParentRotation.Yaw, ParentRotation.Roll);
		DirectX::XMVECTOR NewPosition = DirectX::XMVector3Rotate(Position, Rotation);
		DirectX::XMFLOAT3 RotatedPos;
		DirectX::XMStoreFloat3(&RotatedPos, NewPosition);

		return Parent->GetWorldPosition() + Vector(RotatedPos.x, RotatedPos.y, RotatedPos.z);
	}
	else
		return _RelativeTransform._Position;
}

Vector WorldComponent::GetRelativePosition() const
{
	return _RelativeTransform._Position;
}

void WorldComponent::SetWorldPosition(Vector NewPosition)
{
	Vector CurrentParentWorld;
	WorldComponent* Parent = GetParentComponent();
	while (Parent)
	{
		CurrentParentWorld += Parent->_RelativeTransform._Position;
		Parent = Parent->GetParentComponent();
	}

	_RelativeTransform._Position = NewPosition - CurrentParentWorld;

	// Update the physics transform. Data isnt grabbed from the motion state at runtime unless object is kinematic
	if (_RigidBody)
	{
		btTransform NewTransform;
		//We assume that the object is the root component
		NewTransform.setOrigin(btVector3(_RelativeTransform._Position.X, _RelativeTransform._Position.Y, _RelativeTransform._Position.Z));

		//Convert to radians for now, need to do rotations better lul
		btQuaternion Rotation;
		Rotator RadiansRotation = Rotator::AsRadians(_RelativeTransform._Rotation);
		Rotation.setEuler(RadiansRotation.Yaw, RadiansRotation.Pitch, RadiansRotation.Roll);
		NewTransform.setRotation(Rotation);


		_RigidBody->setWorldTransform(NewTransform);
		//_RigidBody->clearForces();
		//_RigidBody->setLinearVelocity(btVector3(0.0f, 0.0f, 0.0f));
		_RigidBody->activate();
	}

}

void WorldComponent::SetRelativePosition(Vector NewPosition)
{
	_RelativeTransform._Position = NewPosition;
}

void WorldComponent::AddRelativePosition(Vector Position)
{
	_RelativeTransform._Position += Position;
}

Rotator WorldComponent::GetWorldRotation() const
{
	WorldComponent* Parent = GetParentComponent();
	return Parent ? Parent->GetWorldRotation() + _RelativeTransform._Rotation : _RelativeTransform._Rotation;
}

Rotator WorldComponent::GetRelativeRotation() const
{
	return _RelativeTransform._Rotation;
}

void WorldComponent::SetWorldRotation(Rotator NewRotation)
{
	Rotator CurrentParentWorld;

	if (WorldComponent* Parent = GetParentComponent())
		CurrentParentWorld = Parent->GetWorldRotation();

	_RelativeTransform._Rotation = NewRotation - CurrentParentWorld;

	//TODO: UpdatePhysics Movement
}

void WorldComponent::SetRelativeRotation(Rotator NewRotation)
{
	_RelativeTransform._Rotation = NewRotation;
	//TODO: UpdatePhysics Movement
}

void WorldComponent::AddRelativeRotation(Rotator Rotation)
{
	_RelativeTransform._Rotation += Rotation;
}

Vector WorldComponent::GetWorldScale() const
{
	WorldComponent* Parent = GetParentComponent();
	return Parent ? Parent->GetWorldScale() * _RelativeTransform._Scale : _RelativeTransform._Scale;
}

Vector WorldComponent::GetRelativeScale() const
{
	return _RelativeTransform._Scale;
}

void WorldComponent::SetWorldScale(Vector NewScale)
{
	Vector CurrentParentWorld;
	WorldComponent* PointedComp = GetParentComponent();
	while (PointedComp)
	{
		CurrentParentWorld *= PointedComp->_RelativeTransform._Scale;
		PointedComp = PointedComp->GetParentComponent();
	}

	_RelativeTransform._Scale = NewScale - CurrentParentWorld;
}

void WorldComponent::SetRelativeScale(Vector NewScale)
{
	_RelativeTransform._Scale = NewScale;
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
	SetWorldPosition(NewTransform._Position);
	SetWorldRotation(NewTransform._Rotation);
	SetWorldScale(NewTransform._Scale);
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

Vector* WorldComponent::GetWriteablePosition()
{
	return &_RelativeTransform._Position;
}
Rotator* WorldComponent::GetWriteableRotation()
{
	return &_RelativeTransform._Rotation;
}
Vector* WorldComponent::GetWriteableScale()
{
	return &_RelativeTransform._Scale;
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
	//Convert transform into BT Quaternion
	btQuaternion Rotation;
	Rotator Rot = Rotator::AsRadians(GetWorldRotation());
	Vector Pos = GetWorldPosition();
	Rotation.setEulerZYX(Rot.Roll, Rot.Yaw, Rot.Pitch);

	//Setup the motion state
	btVector3 Position = btVector3(Pos.X, Pos.Y, Pos.Z);
	//FOR NOW WE ASSUME THAT THE PHYSICS COMPONENT IS THE ROOT COMPONENT
	MotionState* motionState = new MotionState(&_RelativeTransform._Position, &_RelativeTransform._Rotation);

	//Initialise the mass and intertia values
	btScalar bodyMass = _SimulatePhysics ? 20.0f : 0.0f;
	btVector3 bodyInertia;
	_CollisionShape->calculateLocalInertia(bodyMass, bodyInertia);

	//Create the construction info for the body
	btRigidBody::btRigidBodyConstructionInfo bodyCI =
		btRigidBody::btRigidBodyConstructionInfo(bodyMass, motionState, _CollisionShape, bodyInertia);
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
	return _CollisionShape;
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
	return _CollisionShape;
}

void WorldComponent::Serialize(std::ofstream* Archive)
{
	//Component Class
	std::string ClassName = typeid(WorldComponent).name();
	Archive->write(ClassName.c_str(), sizeof(char) * 32);

	//Name of Component (TODO: Max character length)
	Archive->write(GetName().c_str(), sizeof(char) * 32);

	//Name of parent component
	auto Parent = GetParentComponent();
	if(Parent)
		Archive->write(Parent->GetName().c_str(), sizeof(char) * 32);
	else
		Archive->write("None", sizeof(char) * 32);

	//Write the component transform
	Archive->write(reinterpret_cast<char*>(&GetRelativeTransform()), sizeof(Transform));

	SerializeChildren(Archive);
}

void WorldComponent::SerializeChildren(std::ofstream* Archive)
{
	//Write all child components
	for (auto& Comp : GetChildren())
	{
		Comp->Serialize(Archive);
	}
}

void WorldComponent::Deserialize(std::ifstream* Archive, Actor* NewParent)
{
	//Set the actor name
	char ActorName[32] = "";
	Archive->read(ActorName, sizeof(char) * 32);
	SetName(ActorName);

	SetParent(NewParent);

	DeserializeChildren(Archive, NewParent);
}

void WorldComponent::DeserializeChildren(std::ifstream* Archive, Actor* NewParent)
{
	for (auto& Comp : _Children)
	{
		Comp->Deserialize(Archive, NewParent);
	}
}


