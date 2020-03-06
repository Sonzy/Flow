#include "Flowpch.h"
#include "WorldObject.h"
#include "Components\WorldComponent.h"

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
}