#include "Flowpch.h"
#include "WorldComponent.h"
#include "Flow\GameFramework\WorldObject.h"

namespace Flow
{
	WorldComponent::WorldComponent()
		: Component("Unnamed WorldComponent"), m_ParentComponent(nullptr)
	{
	}

	WorldComponent::WorldComponent(const std::string& Name)
		: Component(Name), m_ParentComponent(nullptr)
	{
	}

	void WorldComponent::Tick(float DeltaTime)
	{
		for (auto& Child : m_Children)
		{
			Child->Tick(DeltaTime);
		}
	}

	Vector WorldComponent::GetWorldLocation()
	{
		WorldComponent* Parent = GetParentComponent();
		return Parent ? Parent->GetWorldLocation() + m_RelativeLocation : m_RelativeLocation;
	}

	void WorldComponent::SetWorldLocation(Vector& NewLocation)
	{
		Vector CurrentParentWorld;
		
		while (WorldComponent* Parent = GetParentComponent())
		{
			CurrentParentWorld += Parent->m_RelativeLocation;
		}

		m_RelativeLocation = NewLocation - CurrentParentWorld;
	}

	void WorldComponent::SetRelativeLocation(const Vector& NewLocation)
	{
		m_RelativeLocation = NewLocation;
	}

	void WorldComponent::Render()
	{
		for (auto Child : m_Children)
		{
			if (WorldComponent* WorldChild = static_cast<WorldComponent*>(Child))
				WorldChild->Render();
		}
	}
}


