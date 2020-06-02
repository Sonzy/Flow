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
	if (CollisionEnabled())
	{
		InitialisePhysics();
	}
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
	return _SimulatePhysics;
}

bool Actor::CollisionEnabled()
{
	return _HasCollision;
}

void Actor::InitialisePhysics()
{
	_RootComponent->InitialisePhysics();
	_SimulatePhysics ? World::GetWorld()->AddPhysicsObject(_RootComponent->GetRigidBody()) : World::GetWorld()->AddCollisionObject(_RootComponent->GetRigidBody());
}

btRigidBody* Actor::GetRigidBody()
{
	return _RootComponent->GetRigidBody();
}

void Actor::DrawDetailsWindow(bool bDontUpdate)
{
	ImGui::Checkbox("Is Visible", &_Visible);
}

void Actor::SetVisibility(bool Visible)
{
	_Visible = Visible;
}