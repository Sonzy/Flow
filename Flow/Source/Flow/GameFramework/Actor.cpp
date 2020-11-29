#include "Flowpch.h"

#include "Actor.h"
#include "Components\WorldComponent.h"

#include "ThirdParty\ImGui\imgui.h"
#include "ThirdParty\ImGui\misc\cpp\imgui_stdlib.h"

#include "GameFramework\World.h"
#include "Editor/UIComponents/Inspector.h"
#include "Editor/Windows/SpawnWindow.h"

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

void Actor::Serialize(std::ofstream* Archive)
{
	//Actor class
	std::string ClassName = typeid(Actor).name();
	Archive->write(ClassName.c_str(), sizeof(char) * 32);

	//Name of actor (TODO: Max character length)
	Archive->write(GetName().c_str(), sizeof(char) * 32);

	//Save whether to load components
	bool HasRoot = m_RootComponent;
	Archive->write(reinterpret_cast<char*>(&HasRoot), sizeof(bool));

	SerializeComponents(Archive);
}

void Actor::SerializeComponents(std::ofstream* Archive)
{
	if (auto Root = GetRootComponent())
	{
		Root->Serialize(Archive);
		Root->SerializeChildren(Archive);
	}
}

void Actor::Deserialize(std::ifstream* Archive)
{
	//Set the actor name
	char ActorName[32] = "";
	Archive->read(ActorName, sizeof(char) * 32);
	SetName(ActorName);

	//Check if the actor had any components
	bool HasRoot = false;
	Archive->read(reinterpret_cast<char*>(&HasRoot), sizeof(bool));

	if (HasRoot == true)
	{
		DeserializeComponents(Archive);
	}
}

void Actor::DeserializeComponents(std::ifstream* Archive)
{
	//Get the UID for the class
	char ActorClassID[32] = "";
	Archive->read(ActorClassID, sizeof(char) * 32);

	WorldComponent* NewRoot = ClassFactory::Get().CreateObjectFromID<WorldComponent>(std::string(ActorClassID));
	if (!NewRoot)
	{
		FLOW_ENGINE_ERROR("Tried to load a component that had an invalid class");
		return;
	}

	m_RootComponent = NewRoot;
	NewRoot->Deserialize(Archive, this);
	NewRoot->DeserializeChildren(Archive, this);
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