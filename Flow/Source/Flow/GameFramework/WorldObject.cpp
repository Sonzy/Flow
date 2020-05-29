#include "Flowpch.h"
#include "WorldObject.h"
#include "Components\WorldComponent.h"
#include "ThirdParty\ImGui\imgui.h"

#include "ThirdParty\ImGui\misc\cpp\imgui_stdlib.h"
#include "Flow\GameFramework\World.h"

WorldObject::WorldObject()
	: _RootComponent(nullptr)
{

}

WorldObject::WorldObject(const std::string& Name)
	: GameObject(Name), _RootComponent(nullptr)
{
}

WorldObject::~WorldObject()
{
	_RootComponent = nullptr;
	FLOW_ENGINE_LOG("WorldObject::~WorldObject");
}

void WorldObject::BeginPlay()
{
	if (CollisionEnabled())
	{
		InitialisePhysics();
	}
}

void WorldObject::Tick(float DeltaTime)
{
	GameObject::Tick(DeltaTime);

	if (_RootComponent)
		_RootComponent->Tick(DeltaTime);
}
WorldComponent* WorldObject::GetRootComponent()
{
	return _RootComponent;
}

Vector WorldObject::GetLocation()
{
	if (!_RootComponent)
		return Vector();

	return _RootComponent->GetRelativePosition();
}

Vector WorldObject::GetScale()
{
	if (!_RootComponent)
		return Vector();

	return _RootComponent->GetRelativeScale();
}

Rotator WorldObject::GetRotation()
{
	if (!_RootComponent)
		return Rotator();

	return _RootComponent->GetRelativeRotation();
}

void WorldObject::Render()
{
	if (_RootComponent && _Visible)
		_RootComponent->Render();
}

bool WorldObject::IsSimulatingPhysics()
{
	return _SimulatePhysics;
}

bool WorldObject::CollisionEnabled()
{
	return _HasCollision;
}

void WorldObject::InitialisePhysics()
{
	_RootComponent->InitialisePhysics();
	_SimulatePhysics ? World::GetWorld()->AddPhysicsObject(_RootComponent->GetRigidBody()) : World::GetWorld()->AddCollisionObject(_RootComponent->GetRigidBody());
}

btRigidBody* WorldObject::GetRigidBody()
{
	return _RootComponent->GetRigidBody();
}

void WorldObject::DrawDetailsWindow(bool bDontUpdate)
{
	ImGui::InputText("ObjectName", &_ObjectName);

	ImGui::Separator(); //==========================================

	//Display Component node tree
	if (ImGui::TreeNode(_RootComponent->GetName().c_str()))
	{
		for (auto Child : _RootComponent->GetChildren())
		{
			ImGui::Text(Child->GetName().c_str());
		}

		ImGui::TreePop();
	}

	ImGui::Separator(); //==========================================



	//Display World Object Transform
	bool bUpdate = false;
	bUpdate |= ImGui::InputFloat3("Position", (float*)_RootComponent->GetWriteablePosition(), 1, ImGuiInputTextFlags_EnterReturnsTrue);
	bUpdate |= ImGui::InputFloat3("Rotation", (float*)_RootComponent->GetWriteableRotation(), 1, ImGuiInputTextFlags_EnterReturnsTrue);
	bUpdate |= ImGui::InputFloat3("Scale", (float*)_RootComponent->GetWriteableScale(), 1, ImGuiInputTextFlags_EnterReturnsTrue);

	//Update Object Transform
	if (bUpdate && !bDontUpdate)
	{
		//TODO: Move physics body
		//if (StaticMeshComponent* Comp = reinterpret_cast<StaticMeshComponent*>(_RootComponent))
		//	Comp->MovePhysicsBody(_RootComponent->GetRelativeTransform());
	}

	ImGui::Checkbox("Is Visible", &_Visible);
}

void WorldObject::SetVisibility(bool Visible)
{
	_Visible = Visible;
}