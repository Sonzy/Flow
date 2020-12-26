#include "Flowpch.h"

#include "Actor.h"
#include "Components\WorldComponent.h"

#include "ThirdParty\ImGui\imgui.h"
#include "ThirdParty\ImGui\misc\cpp\imgui_stdlib.h"

#include "GameFramework\World.h"
#include "Editor/UIComponents/Inspector.h"
#include "Editor/UIComponents/SpawnWindow.h"

#include <yaml-cpp/yaml.h>

#include "Utils/ComponentHelper.h"

Actor::Actor()
	: m_RootComponent(nullptr)
{

}

Actor::Actor(const std::string& Name)
	: GameObject(Name)
	, m_RootComponent(nullptr)
{
}

Actor::~Actor()
{
	FLOW_ENGINE_LOG("Actor::~Actor");
}

void Actor::BeginPlay()
{
	if (!m_RootComponent)
	{
		FLOW_ENGINE_WARNING("Actor::BeginPlay: Actor ' %s ' has no root component", GetName().c_str());
		return;
	}
	m_RootComponent->BeginPlay();
}

void Actor::EditorBeginPlay()
{
	if (!m_RootComponent)
	{
		FLOW_ENGINE_WARNING("Actor::EditorBeginPlay: Actor ' %s ' has no root component", GetName().c_str());
		return;
	}
	m_RootComponent->EditorBeginPlay();
}

void Actor::OnViewportSelected()
{
}

void Actor::OnViewportDeselected()
{
}

void Actor::Tick(float DeltaTime)
{
	GameObject::Tick(DeltaTime);

	if (m_RootComponent)
		m_RootComponent->Tick(DeltaTime);
}
WorldComponent* Actor::SetRootComponent(WorldComponent* NewRoot)
{
	WorldComponent* OldRoot = m_RootComponent;
	m_RootComponent = NewRoot;
	return OldRoot;
}
WorldComponent* Actor::GetRootComponent() const
{
	return m_RootComponent;
}

Vector3 Actor::GetLocation()
{
	if (!m_RootComponent)
		return Vector3();

	return m_RootComponent->GetRelativePosition();
}

Vector3 Actor::GetScale()
{
	if (!m_RootComponent)
		return Vector3();

	return m_RootComponent->GetRelativeScale();
}

Rotator Actor::GetRotation()
{
	if (!m_RootComponent)
		return Rotator();

	return m_RootComponent->GetRelativeRotation();
}

void Actor::Render()
{
	PROFILE_FUNCTION();

	if (m_RootComponent && m_Visible)
	{
		m_RootComponent->Render();
	}
}

bool Actor::IsSimulatingPhysics()
{
	return m_RootComponent ? m_RootComponent->IsSimulatingPhysics() : false;
}

bool Actor::CollisionEnabled()
{
	return m_RootComponent ? m_RootComponent->HasCollision() : false;
}

void Actor::DestroyPhysics()
{
	//TODO: Check more than the root component
	if (m_RootComponent == nullptr)
	{
		return;
	}

	m_RootComponent->DestroyPhysics();
}

void Actor::DrawDetailsWindow(bool bDontUpdate)
{
	ImGui::Checkbox("Is Visible", &m_Visible);
}

void Actor::SetVisibility(bool Visible)
{
	m_Visible = Visible;
}

std::vector<WorldComponent*> Actor::GetComponents() const
{
	//TODO: Remove this
	std::vector<WorldComponent*> Components;

	if (m_RootComponent != nullptr)
	{
		m_RootComponent->BuildChildTree(Components);
	}

	return Components;
}

Component* Actor::GetComponentByName(const std::string& Name) const
{
	if (auto Root = GetRootComponent())
	{
		return Root->GetChildByName(Name);
	}
	else
		return nullptr;
}

void Actor::Serialize(YAML::Emitter& Archive)
{
	GameObject::Serialize(Archive);
	
	Archive << YAML::Key << "Actor";
	Archive << YAML::BeginMap;
	{
		Archive << YAML::Key << "Components";
		Archive << YAML::Value << YAML::BeginSeq;
		{
			ComponentHelper::ComponentRecursion([&Archive](WorldComponent* component){ Archive << component->GetGuid(); }, m_RootComponent);
		}
		Archive << YAML::EndSeq;
	}
	Archive << YAML::EndMap;
}

void Actor::Deserialize(YAML::Node& Archive)
{
	GameObject::Deserialize(Archive);

	if (YAML::Node actorNode = Archive["Actor"])
	{
		if (YAML::Node node = actorNode["Components"])
		{
			//Check if we have already initialised children that havent had their parent set
			World* world = World::Get();
			for (YAML::iterator::value_type child : node)
			{
				WorldComponent* childComponent = world->FindComponent<WorldComponent>(child.as<FGUID>());
				if (childComponent == nullptr)
				{
					continue;
				}

				childComponent->SetParent(this);
			}
		}
	}
}

void Actor::DrawInspectionTree(WorldComponent* CurrentInspectedComponent, bool DontOpenTree)
{
	const ImVec2 ButtonSize = ImVec2(100, 12);
	const float RightOffset = 20;

	bool Popped = false;
	ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 0.0f);

	if (ImGui::BeginChild("Component Details", ImVec2(0.0f, 100.0f), true))
	{
		Popped = true;
		ImGui::PopStyleVar();

		//Add new component
		if (ImGui::BeginPopupContextWindow())
		{
			SpawnWindow::DrawSpawnContextWindow(this);

			ImGui::EndPopup();
		}

		//Draw other components
		if (m_RootComponent != nullptr)
		{
			m_RootComponent->DrawComponentInActorTreeRecursive();
		}
	}
	ImGui::EndChild();

	if (Popped != true)
	{
		ImGui::PopStyleVar();
	}
}