#include "Flowpch.h"
#include "WorldObject.h"
#include "Components\WorldComponent.h"
#include "ThirdParty\ImGui\imgui.h"

//TODO: Remove
#include "Components\StaticMeshComponent.h"

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

	void WorldObject::Render()
	{
		if(m_RootComponent)
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
	}

	btRigidBody* WorldObject::GetRigidBody()
	{
		return m_RootComponent->GetRigidBody();
	}

	void WorldObject::DrawDetailsWindow()
	{
		ImGui::Text(m_ObjectName.c_str());

		ImGui::Separator();

		if (ImGui::TreeNode(m_RootComponent->GetName().c_str()))
		{
			for (auto Child : m_RootComponent->GetChildren())
			{
				ImGui::Text(Child->GetName().c_str());
			}

			ImGui::TreePop();
		}

		ImGui::Separator();

		bool bUpdate = false;
		bUpdate |= ImGui::InputFloat3("Position", (float*)m_RootComponent->GetWriteablePosition(), 1);
		bUpdate |= ImGui::InputFloat3("Rotation", (float*)m_RootComponent->GetWriteableRotation(), 1);
		bUpdate |= ImGui::InputFloat3("Scale", (float*)m_RootComponent->GetWriteableScale(), 1);

		if (bUpdate)
		{
			//TEMP CAST
			if (StaticMeshComponent* Comp = reinterpret_cast<StaticMeshComponent*>(m_RootComponent))
				Comp->MovePhysicsBody(m_RootComponent->GetRelativeTransform());
		}
	}
}