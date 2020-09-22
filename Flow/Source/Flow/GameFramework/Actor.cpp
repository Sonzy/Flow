#include "Flowpch.h"
#include "Actor.h"
#include "Components\WorldComponent.h"
#include "ThirdParty\ImGui\imgui.h"

#include "ThirdParty\ImGui\misc\cpp\imgui_stdlib.h"
#include "Flow\GameFramework\World.h"

//#include "Flow/GameFramework/Other/ClassFactory.h"

Actor::Actor()
	: _RootComponent(nullptr)
{

}

Actor::Actor(const std::string& Name)
	: GameObject(Name), _RootComponent(nullptr)
{
}

Actor::~Actor()
{
	FLOW_ENGINE_LOG("Actor::~Actor");
}

void Actor::BeginPlay()
{
	if (!_RootComponent)
	{
		FLOW_ENGINE_WARNING("Actor::BeginPlay: Actor ' {0} ' has no root component", GetName());
		return;
	}
	_RootComponent->BeginPlay();
}

void Actor::EditorBeginPlay()
{
	if (!_RootComponent)
	{
		FLOW_ENGINE_WARNING("Actor::EditorBeginPlay: Actor ' {0} ' has no root component", GetName());
		return;
	}
	_RootComponent->EditorBeginPlay();
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

	if (_RootComponent)
		_RootComponent->Tick(DeltaTime);
}
WorldComponent* Actor::GetRootComponent() const
{
	return _RootComponent;
}

Vector3 Actor::GetLocation()
{
	if (!_RootComponent)
		return Vector3();

	return _RootComponent->GetRelativePosition();
}

Vector3 Actor::GetScale()
{
	if (!_RootComponent)
		return Vector3();

	return _RootComponent->GetRelativeScale();
}

Rotator Actor::GetRotation()
{
	if (!_RootComponent)
		return Rotator();

	return _RootComponent->GetRelativeRotation();
}

void Actor::Render()
{
	PROFILE_FUNCTION();

	if (_RootComponent && _Visible)
		_RootComponent->Render();
}

bool Actor::IsSimulatingPhysics()
{
	return _RootComponent ? _RootComponent->IsSimulatingPhysics() : false;
}

bool Actor::CollisionEnabled()
{
	return _RootComponent ? _RootComponent->HasCollision() : false;
}

void Actor::DrawDetailsWindow(bool bDontUpdate)
{
	ImGui::Checkbox("Is Visible", &_Visible);
}

void Actor::SetVisibility(bool Visible)
{
	_Visible = Visible;
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
	bool HasRoot = _RootComponent;
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

	_RootComponent = NewRoot;
	NewRoot->Deserialize(Archive, this);
	NewRoot->DeserializeChildren(Archive, this);
}

