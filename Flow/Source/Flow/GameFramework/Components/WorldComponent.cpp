#include "pch.h"
#include "WorldComponent.h"

#include "ThirdParty\ImGui\imgui.h"

#include "Bullet/btBulletCollisionCommon.h"
#include "Bullet/btBulletDynamicsCommon.h"
#include "Physics/MotionState.h"

#include "GameFramework\Actor.h"
#include "GameFramework/World.h"
#include "GameFramework/Other/ClassFactory.h"

#include "Editor/UIComponents/Inspector.h"
#include "Editor/Editor.h"
#include "Editor/Tools\SelectionTool.h"

#include <yaml-cpp/yaml.h>
#include "Framework/Utils/YamlSerializer.h"

WorldComponent::WorldComponent()
	: WorldComponent("Unnamed WorldComponent")
{
}

WorldComponent::WorldComponent(const std::string& Name)
	: Component(Name)
	, m_ParentComponent(nullptr)
	, m_RigidBody(nullptr)
	, m_CollisionShape(nullptr)
	, m_MotionState(nullptr)
	, m_CollisionEnabled(true)
{
}

WorldComponent::~WorldComponent()
{
	m_Children.clear();

	if (m_RigidBody != nullptr)			delete m_RigidBody;
	if (m_CollisionShape != nullptr)	delete m_CollisionShape;
	if (m_MotionState != nullptr)		delete m_MotionState;

	m_RigidBody = nullptr;
	m_CollisionShape = nullptr;
	m_MotionState = nullptr;
}

#if WITH_EDITOR
void WorldComponent::EditorBeginPlay()
{
	for (auto& Child : m_Children)
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
void WorldComponent::IconUpdate(IconManager& iconManager)
{
}
#endif

void WorldComponent::BeginPlay()
{
	for (auto& Child : m_Children)
	{
		Child->BeginPlay();
	}
}

void WorldComponent::Tick(float DeltaTime)
{
	PROFILE_FUNCTION();

	for (auto& Child : m_Children)
	{
		Child->Tick(DeltaTime);
	}
}

void WorldComponent::AddChild(WorldComponent* Child)
{
	CHECK_RETURN(!Child, "WorldComponent::AddChild: New Child was nullptr");
	m_Children.push_back(Child);
	Child->SetParentComponent(this);
}

Vector3 WorldComponent::GetWorldPosition() const
{
	WorldComponent* Parent = GetParentComponent(); //TODO: Rotate this by the parents rotation
	//return Parent ? Parent->GetWorldPosition() + m_RelativeTransform._Position : m_RelativeTransform._Position;

	if (Parent)
	{
		Rotator ParentRotation = Rotator::AsRadians(Parent->GetWorldRotation());

		DirectX::XMVECTOR Position = DirectX::XMLoadFloat3(reinterpret_cast<const DirectX::XMFLOAT3*>(&m_RelativeTransform.m_Position));
		DirectX::XMVECTOR Rotation = DirectX::XMQuaternionRotationRollPitchYaw(ParentRotation.Pitch, ParentRotation.Yaw, ParentRotation.Roll);
		DirectX::XMVECTOR NewPosition = DirectX::XMVector3Rotate(Position, Rotation);
		DirectX::XMFLOAT3 RotatedPos;
		DirectX::XMStoreFloat3(&RotatedPos, NewPosition);

		return Parent->GetWorldPosition() + Vector3(RotatedPos.x, RotatedPos.y, RotatedPos.z);
	}
	else
		return m_RelativeTransform.m_Position;
}

Vector3 WorldComponent::GetRelativePosition() const
{
	return m_RelativeTransform.m_Position;
}

void WorldComponent::SetWorldPosition(Vector3 NewPosition)
{
	Vector3 CurrentParentWorld;
	WorldComponent* Parent = GetParentComponent();
	while (Parent)
	{
		CurrentParentWorld += Parent->m_RelativeTransform.m_Position;
		Parent = Parent->GetParentComponent();
	}

	m_RelativeTransform.m_Position = NewPosition - CurrentParentWorld;

	// Update the physics transform. Data isnt grabbed from the motion state at runtime unless object is kinematic
	if (m_RigidBody)
	{
		btTransform NewTransform;
		//We assume that the object is the root component
		NewTransform.setOrigin(btVector3(m_RelativeTransform.m_Position));

		//Convert to radians for now, need to do rotations better lul
		btQuaternion Rotation;
		Rotator RadiansRotation = Rotator::AsRadians(m_RelativeTransform.m_Rotation);
		Rotation.setEuler(RadiansRotation.Yaw, RadiansRotation.Pitch, RadiansRotation.Roll);
		NewTransform.setRotation(Rotation);


		m_RigidBody->setWorldTransform(NewTransform);
		//m_RigidBody->clearForces();
		//m_RigidBody->setLinearVelocity(btVector3(0.0f, 0.0f, 0.0f));
		m_RigidBody->activate();
	}

}

void WorldComponent::SetRelativePosition(Vector3 NewPosition)
{
	m_RelativeTransform.m_Position = NewPosition;
}

void WorldComponent::AddRelativePosition(Vector3 Position)
{
	m_RelativeTransform.m_Position += Position;
}

Rotator WorldComponent::GetWorldRotation() const
{
	WorldComponent* Parent = GetParentComponent();
	return Parent ? Parent->GetWorldRotation() + m_RelativeTransform.m_Rotation : m_RelativeTransform.m_Rotation;
}

Rotator WorldComponent::GetRelativeRotation() const
{
	return m_RelativeTransform.m_Rotation;
}

void WorldComponent::SetWorldRotation(Rotator NewRotation)
{
	Rotator CurrentParentWorld;

	if (WorldComponent* Parent = GetParentComponent())
		CurrentParentWorld = Parent->GetWorldRotation();

	m_RelativeTransform.m_Rotation = NewRotation - CurrentParentWorld;
	//TODO: UpdatePhysics Movement
}

void WorldComponent::SetRelativeRotation(Rotator NewRotation)
{
	m_RelativeTransform.m_Rotation = NewRotation;
	//TODO: UpdatePhysics Movement
}

void WorldComponent::AddRelativeRotation(Rotator Rotation)
{
	m_RelativeTransform.m_Rotation += Rotation;
}

Vector3 WorldComponent::GetWorldScale() const
{
	WorldComponent* Parent = GetParentComponent();
	return Parent ? Parent->GetWorldScale() * m_RelativeTransform.m_Scale : m_RelativeTransform.m_Scale;
}

Vector3 WorldComponent::GetRelativeScale() const
{
	return m_RelativeTransform.m_Scale;
}

void WorldComponent::SetWorldScale(Vector3 NewScale)
{
	//TODO: Nan check
	if (NewScale.IsValid() == false)
	{
		return;
	}

	Vector3 CurrentParentWorld;
	WorldComponent* PointedComp = GetParentComponent();
	while (PointedComp)
	{
		CurrentParentWorld *= PointedComp->m_RelativeTransform.m_Scale;
		PointedComp = PointedComp->GetParentComponent();
	}

	m_RelativeTransform.m_Scale = NewScale - CurrentParentWorld;

	UpdateCollisionScale();
}

void WorldComponent::SetRelativeScale(Vector3 NewScale)
{
	m_RelativeTransform.m_Scale = NewScale;

	UpdateCollisionScale();
}

void WorldComponent::AddRelativeScale(Vector3 NewScale)
{
	m_RelativeTransform.m_Scale += NewScale;

	UpdateCollisionScale();
}

Transform WorldComponent::GetWorldTransform() const
{
	return Transform(GetWorldPosition(), GetWorldRotation(), GetWorldScale());
}

Transform WorldComponent::GetRelativeTransform() const
{
	return m_RelativeTransform;
}

void WorldComponent::SetWorldTransform(Transform NewTransform)
{
	SetWorldPosition(NewTransform.m_Position);
	SetWorldRotation(NewTransform.m_Rotation);
	SetWorldScale(NewTransform.m_Scale);
}

void WorldComponent::SetRelativeTransform(Transform NewTransform)
{
	m_RelativeTransform = NewTransform;
}

Matrix4x4 WorldComponent::GetTransformationMatrix()
{
	//TODO: Remove DX dependancy

	Transform WorldTransform = GetWorldTransform();
	Rotator RadiansRotation = Rotator::AsRadians(WorldTransform.m_Rotation);

	DirectX::XMMATRIX mat =	
		DirectX::XMMatrixScaling(WorldTransform.m_Scale.x, WorldTransform.m_Scale.y, WorldTransform.m_Scale.z) *
		DirectX::XMMatrixRotationQuaternion(Maths::EulersToQuaternion(RadiansRotation)) *
		//TODO: XMMAtrixRotationRollPitchYaw doesn't work here. Not sure what is different between the conversion formulae
		DirectX::XMMatrixTranslation(WorldTransform.m_Position.x, WorldTransform.m_Position.y, WorldTransform.m_Position.z);

	Matrix4x4 outMat;
	DirectX::XMStoreFloat4x4(reinterpret_cast<DirectX::XMFLOAT4X4*>(&outMat), mat); //TODO: Check this is fine
	return outMat;
}

void WorldComponent::Render()
{
	PROFILE_CURRENT_SCOPE("Render Children");

	for (auto Child : m_Children)
	{
		if (Child)
			Child->Render();
	}
}

Vector3* WorldComponent::GetWriteablePosition()
{
	return &m_RelativeTransform.m_Position;
}
Rotator* WorldComponent::GetWriteableRotation()
{
	return &m_RelativeTransform.m_Rotation;
}
Vector3* WorldComponent::GetWriteableScale()
{
	return &m_RelativeTransform.m_Scale;
}

#if WITH_EDITOR

void WorldComponent::DrawDetailsWindow(bool bDontUpdate)
{
}

void WorldComponent::DrawComponentDetailsWindow()
{
	Component::DrawComponentDetailsWindow();

	ImGui::Checkbox("Simulate Physics", &m_SimulatePhysics);
}

void WorldComponent::DrawComponentInActorTreeRecursive()
{
	//TODO: Need to use some GUIDs
	if (ImGui::TreeNodeEx(m_name.c_str(), ImGuiTreeNodeFlags_DefaultOpen))
	{
		if (ImGui::IsItemClicked())
		{
			Editor::Get().GetTool<SelectionTool>()->SelectComponent(this);
		}

		if (Editor::Get().GetTool<SelectionTool>()->GetSelectedComponent() == this)
		{
			ImGui::SameLine();
			ImGui::TextColored(IMGUI_GREEN, "Selected");
		}

		for (WorldComponent* Child : m_Children)
		{
			Child->DrawComponentInActorTreeRecursive();
		}
		ImGui::TreePop();
	}
}

#endif // WITH_EDITOR

std::vector<WorldComponent*> WorldComponent::GetChildren() const
{
	return m_Children;
}

WorldComponent* WorldComponent::GetChildByName(const std::string& ChildName)
{
	for (auto& Comp : m_Children)
	{
		if (Comp->GetName() == ChildName)
		{
			return Comp;
		}
	}

	return nullptr;
}

void WorldComponent::BuildChildTree(std::vector<WorldComponent*>& Container, bool IncludeSelf)
{
	if (IncludeSelf == true)
	{
		Container.push_back(this);
	}

	for (WorldComponent* Component : m_Children)
	{
		BuildChildTree(Container);
	}
}

void WorldComponent::SetVisibility(bool Visible)
{
	m_Visible = Visible;
}
bool WorldComponent::IsVisible() const
{
	return m_Visible;
}

void WorldComponent::InitialisePhysics()
{
	m_SimulatePhysics = false;
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

	//TODO: Manage memory, currently leaking
	//FOR NOW WE ASSUME THAT THE PHYSICS COMPONENT IS THE ROOT COMPONENT
	MotionState* motionState = new MotionState(&m_RelativeTransform.m_Position, &m_RelativeTransform.m_Rotation);

	//Initialise the mass and intertia values
	btScalar bodyMass = m_SimulatePhysics ? 20.0f : 0.0f;
	btVector3 bodyInertia;
	m_CollisionShape->calculateLocalInertia(bodyMass, bodyInertia);

	//Create the construction info for the body
	btRigidBody::btRigidBodyConstructionInfo bodyCI =
		btRigidBody::btRigidBodyConstructionInfo(bodyMass, motionState, m_CollisionShape, bodyInertia);
	bodyCI.m_restitution = 0.4f;
	bodyCI.m_friction = 0.5f;
	bodyCI.m_rollingFriction = 0.2f;
	bodyCI.m_spinningFriction = 0.3f;

	m_RigidBody = new btRigidBody(bodyCI);
	m_RigidBody->setUserPointer(this);

	//TODO: Testing flag issues
	m_RigidBody->setCollisionFlags(0);
}

void WorldComponent::UpdateAABB()
{
	if (m_RigidBody)
	{
		World::GetPhysicsWorld()->updateSingleAabb(m_RigidBody);
	}
}

btRigidBody* WorldComponent::GetRigidBody() const
{
	return m_RigidBody;
}

btCollisionShape* WorldComponent::GetCollisionShape() const
{
	return m_CollisionShape;
}

void WorldComponent::SetSimulatePhysics(bool Simulate)
{
	m_SimulatePhysics = Simulate;
}

bool WorldComponent::IsSimulatingPhysics() const
{
	return m_SimulatePhysics;
}

bool WorldComponent::HasCollision() const
{
	return m_CollisionShape;
}

void WorldComponent::UpdateCollisionScale()
{
	if (!m_CollisionEnabled || !m_CollisionShape || !m_RigidBody)
	{
		return;
	}

	m_CollisionShape->setLocalScaling(m_RelativeTransform.m_Scale);
	World::GetPhysicsWorld()->updateSingleAabb(m_RigidBody);
}

void WorldComponent::Serialize(YAML::Emitter& Archive)
{
	Component::Serialize(Archive);

	Archive << YAML::Key << "WorldComponent";
	Archive << YAML::BeginMap;
	{
		Archive << YAML::Key << "ParentActor";
		Archive << YAML::Value << (m_ParentObject != nullptr ? m_ParentObject->GetGuid() : -1);

		Archive << YAML::Key << "IsRoot";
		Archive << YAML::Value << (m_ParentObject != nullptr ? m_ParentObject->GetRootComponent() == this : false);

		Archive << YAML::Key << "ParentComponent";
		Archive << YAML::Value << (m_AttachedComponent ? m_AttachedComponent->GetGuid() : -1);

		Archive << YAML::Key << "ComponentTransform";
		Archive << YAML::Value << GetRelativeTransform();

		Archive << YAML::Key << "SimulatingPhysics";
		Archive << YAML::Value << m_SimulatePhysics;

		Archive << YAML::Key << "Children";
		Archive << YAML::Value << YAML::BeginSeq;
		{
			for (WorldComponent* Child : m_Children)
			{
				Archive << Child->GetGuid();
			}
		}
		Archive << YAML::EndSeq;
	}
	Archive << YAML::EndMap;
}

void WorldComponent::Deserialize(YAML::Node& Archive)
{
	Component::Deserialize(Archive);

	if (YAML::Node node = Archive["WorldComponent"])
	{
		SetParent(World::Get().FindActor(node["ParentActor"].as<FGuid>()));

		//TODO: mebby reimplement. for now the actor handles this
		//if (node["IsRoot"].as<bool>())
		//{
		//	if (m_ParentObject != nullptr)
		//	{
		//		m_ParentObject->SetRootComponent(this);
		//	}
		//	else
		//	{
		//		FLOW_ENGINE_ERROR("WorldComponent::Deserialize: Deserialised a component with a parent that cannot be found");
		//	}
		//}

		SetParentComponent(dynamic_cast<WorldComponent*>(World::Get().FindComponent(node["ParentComponent"].as<FGuid>())));

		SetRelativeTransform(node["ComponentTransform"].as<Transform>());

		m_SimulatePhysics = node["SimulatingPhysics"].as<bool>();

		if (YAML::Node node = Archive["Children"])
		{
			//Check if we have already initialised children that havent had their parent set
			World& world = World::Get();			
			for (YAML::iterator::value_type child : node)
			{
				WorldComponent* childComponent = world.FindComponent<WorldComponent>(child.as<FGuid>());
				if (childComponent == nullptr)
				{
					continue;
				}

				childComponent->SetParentComponent(this);
			}
		}
	}
}

void WorldComponent::DefaultInitialise()
{
}
