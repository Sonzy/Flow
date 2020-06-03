#include "Flowpch.h"
#include "Actor.h"
#include "Components\WorldComponent.h"
#include "ThirdParty\ImGui\imgui.h"

#include "ThirdParty\ImGui\misc\cpp\imgui_stdlib.h"
#include "Flow\GameFramework\World.h"

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
	_RootComponent = nullptr;
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

void Actor::Tick(float DeltaTime)
{
	GameObject::Tick(DeltaTime);

	if (_RootComponent)
		_RootComponent->Tick(DeltaTime);
}
WorldComponent* Actor::GetRootComponent()
{
	return _RootComponent;
}

Vector Actor::GetLocation()
{
	if (!_RootComponent)
		return Vector();

	return _RootComponent->GetRelativePosition();
}

Vector Actor::GetScale()
{
	if (!_RootComponent)
		return Vector();

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