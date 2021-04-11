#include "pch.h"

#include "Actor.h"
#include "Components\WorldComponent.h"

#include "ThirdParty\ImGui\imgui.h"
#include "ThirdParty\ImGui\misc\cpp\imgui_stdlib.h"

#include "GameFramework\World.h"


#include "Framework/Utils/YamlSerializer.h"
#include <yaml-cpp/yaml.h>

#include "Framework/Utils/ComponentHelper.h"

#if WITH_EDITOR
	#include "Editor/Editor.h"
	#include "Editor/Modules/Spawner.h"
	#include "Editor/UIComponents/Inspector.h"
#endif

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
}

void Actor::OnRegistered()
{
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

#if WITH_EDITOR

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

#endif // WITH_EDITOR

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

Vector3 Actor::GetLocation() const
{
	if (!m_RootComponent)
		return Vector3();

	return m_RootComponent->GetRelativePosition();
}

Vector3 Actor::GetScale() const
{
	if (!m_RootComponent)
		return Vector3();

	return m_RootComponent->GetRelativeScale();
}

Rotator Actor::GetRotation() const
{
	if (!m_RootComponent)
		return Rotator();

	return m_RootComponent->GetRelativeRotation();
}

Transform Actor::GetWorldTransform() const
{
	if (m_RootComponent == nullptr)
	{
		return Transform();
	}
	return m_RootComponent->GetRelativeTransform();
}

void Actor::SetWorldTransform(const Transform& transform)
{
	if (m_RootComponent != nullptr)
	{
		return m_RootComponent->SetWorldTransform(transform);
	}
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
		Archive << YAML::Key << "Transform";
		Archive << YAML::Value << GetWorldTransform();

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
			World& world = World::Get();

			bool root = true;
			for (YAML::iterator::value_type child : node)
			{
				WorldComponent* childComponent = world.FindComponent<WorldComponent>(child.as<FGUID>());
				if (childComponent == nullptr)
				{
					continue;
				}

				childComponent->SetParent(this);
				if (root)
				{
					SetRootComponent(childComponent);
					root = false;
					//TODO: Do better
				}
			}
		}

		//TODO: Give it an empty root if we dont have one?

		SetWorldTransform(actorNode["Transform"].as<Transform>()); // Set transform after to make sure we have a root component
	}
}

#if WITH_EDITOR
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
			Editor::Get().GetModule<Spawner>()->DrawComponentSpawnContextWindow(this);

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

#endif WITH_EDITOR