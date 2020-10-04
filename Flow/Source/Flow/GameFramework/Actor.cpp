#include "Flowpch.h"
#include "Actor.h"
#include "Components\WorldComponent.h"
#include "ThirdParty\ImGui\imgui.h"

#include "ThirdParty\ImGui\misc\cpp\imgui_stdlib.h"
#include "Flow\GameFramework\World.h"

//#include "Flow/GameFramework/Other/ClassFactory.h"

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
		FLOW_ENGINE_WARNING("Actor::BeginPlay: Actor ' {0} ' has no root component", GetName());
		return;
	}
	m_RootComponent->BeginPlay();
}

void Actor::EditorBeginPlay()
{
	if (!m_RootComponent)
	{
		FLOW_ENGINE_WARNING("Actor::EditorBeginPlay: Actor ' {0} ' has no root component", GetName());
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
		m_RootComponent->Render();
}

bool Actor::IsSimulatingPhysics()
{
	return m_RootComponent ? m_RootComponent->IsSimulatingPhysics() : false;
}

bool Actor::CollisionEnabled()
{
	return m_RootComponent ? m_RootComponent->HasCollision() : false;
}

void Actor::DrawDetailsWindow(bool bDontUpdate)
{
	ImGui::Checkbox("Is Visible", &m_Visible);
}

void Actor::SetVisibility(bool Visible)
{
	m_Visible = Visible;
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

	if(HasRoot)
		DeserializeComponents(Archive);
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

