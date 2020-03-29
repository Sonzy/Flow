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
		: m_RootComponent(nullptr)
	{

	}

	WorldObject::WorldObject(const std::string& Name)
		: GameObject(Name), m_RootComponent(nullptr)
	{
	}

	WorldObject::~WorldObject()
	{
		m_RootComponent = nullptr;
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
		
		if(m_RootComponent)
			m_RootComponent->Tick(DeltaTime);
	}
	WorldComponent* WorldObject::GetRootComponent()
	{
		return m_RootComponent;
	}

	Vector WorldObject::GetLocation()
	{
		if(!m_RootComponent)
			return Vector();
		
		return m_RootComponent->GetRelativeLocation();
	}

	Vector WorldObject::GetScale()
	{
		if (!m_RootComponent)
			return Vector();

		return m_RootComponent->GetRelativeScale();
	}

	Rotator WorldObject::GetRotation()
	{
		if (!m_RootComponent)
			return Rotator();

		return m_RootComponent->GetRelativeRotation();
	}

	void WorldObject::Render()
	{
		if(m_RootComponent && Visible)
			m_RootComponent->Render();
	}

	bool WorldObject::IsSimulatingPhysics()
	{
		return SimulatePhysics;
	}

	bool WorldObject::CollisionEnabled()
	{
		return HasCollision;
	}

	void WorldObject::InitialisePhysics()
	{
		m_RootComponent->InitialisePhysics();
		SimulatePhysics ? World::GetWorld()->AddPhysicsObject(m_RootComponent->GetRigidBody()) : World::GetWorld()->AddCollisionObject(m_RootComponent->GetRigidBody());
	}

	btRigidBody* WorldObject::GetRigidBody()
	{
		return m_RootComponent->GetRigidBody();
	}

	void WorldObject::DrawDetailsWindow(bool bDontUpdate)
	{
		ImGui::InputText("ObjectName", &m_ObjectName);

		ImGui::Separator(); //==========================================

		//Display Component node tree
		if (ImGui::TreeNode(m_RootComponent->GetName().c_str()))
		{
			for (auto Child : m_RootComponent->GetChildren())
			{
				ImGui::Text(Child->GetName().c_str());
			}

			ImGui::TreePop();
		}

		ImGui::Separator(); //==========================================

		//Display World Object Transform
		bool bUpdate = false;
		bUpdate |= ImGui::InputFloat3("Position", m_RootComponent->GetXPointer(), 1);
		bUpdate |= ImGui::InputFloat3("Rotation", (float*)m_RootComponent->GetWriteableRotation(), 1);
		bUpdate |= ImGui::InputFloat3("Scale", (float*)m_RootComponent->GetWriteableScale(), 1);
		//bUpdate |= ImGui::InputFloat3("Position", (float*)m_RootComponent->GetWriteablePosition(), 1);




		//Update Object Transform
		if (bUpdate && !bDontUpdate)
		{
			//TEMP CAST
			if (StaticMeshComponent* Comp = reinterpret_cast<StaticMeshComponent*>(m_RootComponent))
				Comp->MovePhysicsBody(m_RootComponent->GetRelativeTransform());
		}

		ImGui::Checkbox("Is Visible", &Visible);
	}
}