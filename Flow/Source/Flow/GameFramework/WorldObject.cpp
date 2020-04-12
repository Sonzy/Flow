#include "Flowpch.h"
#include "WorldObject.h"
#include "Components\WorldComponent.h"
#include "ThirdParty\ImGui\imgui.h"

//TODO: Remove
#include "Components\StaticMeshComponent.h"

#include "ThirdParty\ImGui\misc\cpp\imgui_stdlib.h"
#include "Flow\GameFramework\World.h"

namespace Flow
{
	WorldObject::WorldObject()
		: RootComponent_(nullptr)
	{

	}

	WorldObject::WorldObject(const std::string& Name)
		: GameObject(Name), RootComponent_(nullptr)
	{
	}

	WorldObject::~WorldObject()
	{
		RootComponent_ = nullptr;
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
		
		if(RootComponent_)
			RootComponent_->Tick(DeltaTime);
	}
	WorldComponent* WorldObject::GetRootComponent()
	{
		return RootComponent_;
	}

	Vector WorldObject::GetLocation()
	{
		if(!RootComponent_)
			return Vector();
		
		return RootComponent_->GetRelativePosition();
	}

	Vector WorldObject::GetScale()
	{
		if (!RootComponent_)
			return Vector();

		return RootComponent_->GetRelativeScale();
	}

	Rotator WorldObject::GetRotation()
	{
		if (!RootComponent_)
			return Rotator();

		return RootComponent_->GetRelativeRotation();
	}

	void WorldObject::Render()
	{
		if(RootComponent_ && Visible_)
			RootComponent_->Render();
	}

	bool WorldObject::IsSimulatingPhysics()
	{
		return SimulatePhysics_;
	}

	bool WorldObject::CollisionEnabled()
	{
		return HasCollision_;
	}

	void WorldObject::InitialisePhysics()
	{
		RootComponent_->InitialisePhysics();
		SimulatePhysics_ ? World::GetWorld()->AddPhysicsObject(RootComponent_->GetRigidBody()) : World::GetWorld()->AddCollisionObject(RootComponent_->GetRigidBody());
	}

	btRigidBody* WorldObject::GetRigidBody()
	{
		return RootComponent_->GetRigidBody();
	}

	void WorldObject::DrawDetailsWindow(bool bDontUpdate)
	{
		ImGui::InputText("ObjectName", &ObjectName_);

		ImGui::Separator(); //==========================================

		//Display Component node tree
		if (ImGui::TreeNode(RootComponent_->GetName().c_str()))
		{
			for (auto Child : RootComponent_->GetChildren())
			{
				ImGui::Text(Child->GetName().c_str());
			}

			ImGui::TreePop();
		}

		ImGui::Separator(); //==========================================

		

		//Display World Object Transform
		bool bUpdate = false;
		bUpdate |= ImGui::InputFloat3("Position", RootComponent_->GetXPointer(), 1, ImGuiInputTextFlags_EnterReturnsTrue);
		bUpdate |= ImGui::InputFloat3("Rotation", (float*)RootComponent_->GetWriteableRotation(), 1, ImGuiInputTextFlags_EnterReturnsTrue);
		bUpdate |= ImGui::InputFloat3("Scale", (float*)RootComponent_->GetWriteableScale(), 1, ImGuiInputTextFlags_EnterReturnsTrue);
		//bUpdate |= ImGui::InputFloat3("Position", (float*)m_RootComponent->GetWriteablePosition(), 1);

		//Update Object Transform
		if (bUpdate && !bDontUpdate)
		{
			//TEMP CAST
			if (StaticMeshComponent* Comp = reinterpret_cast<StaticMeshComponent*>(RootComponent_))
				Comp->MovePhysicsBody(RootComponent_->GetRelativeTransform());
		}

		ImGui::Checkbox("Is Visible", &Visible_);
	}
}