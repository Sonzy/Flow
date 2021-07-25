// Pch //////////////////////////////////////////////////////////

#include "pch.h"

// Main Include /////////////////////////////////////////////////

#include "Actor.h"

// Includes /////////////////////////////////////////////////////

#include <yaml-cpp/yaml.h>
#include "Components/WorldComponent.h"
#include "Framework/Utils/YamlSerializer.h"
#include "Framework/Utils/ComponentHelper.h"
#include "GameFramework/World.h"
#include "ThirdParty/ImGui/imgui.h"

#if WITH_EDITOR
	#include "Editor/Editor.h"
	#include "Editor/Modules/Spawner.h"
	#include "Editor/UIComponents/Inspector.h"
#endif

// Function Definitions /////////////////////////////////////////

Actor::Actor()
	: Actor("Unnamed Actor")
{
}

Actor::Actor(const std::string& Name)
	: GameObject(Name)
	, m_rootComponent(nullptr)
	, m_currentController(nullptr)
	, m_tickEnabled(true)
	, m_visible(true)
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
	if (m_rootComponent == nullptr)
	{
		FLOW_ENGINE_WARNING("Actor::BeginPlay: Actor ' %s ' has no root component", GetName().c_str());
		return;
	}

	m_rootComponent->BeginPlay();
}

#if WITH_EDITOR

void Actor::EditorBeginPlay()
{
	if (m_rootComponent == nullptr)
	{
		FLOW_ENGINE_WARNING("Actor::EditorBeginPlay: Actor ' %s ' has no root component", GetName().c_str());
		return;
	}
	m_rootComponent->EditorBeginPlay();
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

	if (m_rootComponent != nullptr)
	{
		m_rootComponent->Tick(DeltaTime);
	}
}
WorldComponent* Actor::SetRootComponent(WorldComponent* NewRoot)
{
	WorldComponent* OldRoot = m_rootComponent;
	m_rootComponent = NewRoot;
	return OldRoot;
}
WorldComponent* Actor::GetRootComponent() const
{
	return m_rootComponent;
}

Vector3 Actor::GetLocation() const
{
	if (m_rootComponent == nullptr)
	{
		return Vector3();
	}

	return m_rootComponent->GetRelativePosition();
}

Vector3 Actor::GetScale() const
{
	if (m_rootComponent == nullptr)
	{
		return Vector3();
	}

	return m_rootComponent->GetRelativeScale();
}

Rotator Actor::GetRotation() const
{
	if (m_rootComponent == nullptr)
	{
		return Rotator();
	}

	return m_rootComponent->GetRelativeRotation();
}

Transform Actor::GetWorldTransform() const
{
	if (m_rootComponent == nullptr)
	{
		return Transform();
	}

	return m_rootComponent->GetRelativeTransform();
}

void Actor::SetWorldTransform(const Transform& transform)
{
	if (m_rootComponent != nullptr)
	{
		return m_rootComponent->SetWorldTransform(transform);
	}
}

void Actor::Render()
{
	PROFILE_FUNCTION();

	if (m_rootComponent && m_visible)
	{
		m_rootComponent->Render();
	}
}

bool Actor::IsSimulatingPhysics()
{
	if (m_rootComponent == nullptr)
	{
		return false;
	}

	return m_rootComponent->IsSimulatingPhysics();
}

bool Actor::CollisionEnabled()
{
	if (m_rootComponent == nullptr)
	{
		return false;
	}

	return m_rootComponent->HasCollision();
}

void Actor::DestroyPhysics()
{
	if (m_rootComponent == nullptr)
	{
		return;
	}

	m_rootComponent->DestroyPhysics();
}

void Actor::DrawDetailsWindow(bool bDontUpdate)
{
	ImGui::Checkbox("Is Visible", &m_visible);
}

void Actor::SetVisibility(bool Visible)
{
	m_visible = Visible;
}

std::vector<WorldComponent*> Actor::GetComponents() const
{
	std::vector<WorldComponent*> Components;

	if (m_rootComponent != nullptr)
	{
		m_rootComponent->BuildChildTree(Components);
	}

	return Components;
}

Component* Actor::GetComponentByName(const std::string& Name) const
{
	// TODO: This won't check children more than 1 tree deep

	if (WorldComponent* root = GetRootComponent())
	{
		return root->GetChildByName(Name);
	}

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
			ComponentHelper::ComponentRecursion([&Archive](WorldComponent* component){ Archive << component->GetGuid(); }, m_rootComponent);
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
				WorldComponent* childComponent = world.FindComponent<WorldComponent>(child.as<FGuid>());
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
		if (m_rootComponent != nullptr)
		{
			m_rootComponent->DrawComponentInActorTreeRecursive();
		}
	}
	ImGui::EndChild();

	if (Popped != true)
	{
		ImGui::PopStyleVar();
	}
}

#endif WITH_EDITOR
