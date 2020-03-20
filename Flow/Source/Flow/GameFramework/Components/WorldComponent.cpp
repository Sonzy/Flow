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

	WorldComponent::~WorldComponent()
	{
		m_Children.clear();
	}

	void WorldComponent::Tick(float DeltaTime)
	{
		for (auto& Child : m_Children)
		{
			Child->Tick(DeltaTime);
		}
	}

	void WorldComponent::AddChild(WorldComponent* Child)
	{
		CHECK_RETURN(!Child, "WorldComponent::AddChild: New Child was nullptr");
		m_Children.push_back(Child);
	}

	Vector WorldComponent::GetWorldLocation()
	{
		WorldComponent* Parent = GetParentComponent();
		return Parent ? Parent->GetWorldLocation() + m_RelativeTransform.m_Location : m_RelativeTransform.m_Location;
	}

	Vector WorldComponent::GetRelativeLocation()
	{
		return m_RelativeTransform.m_Location;
	}

	void WorldComponent::SetWorldLocation(Vector NewLocation)
	{
		Vector CurrentParentWorld;
		
		while (WorldComponent* Parent = GetParentComponent())
		{
			CurrentParentWorld += Parent->m_RelativeTransform.m_Location;
		}

		m_RelativeTransform.m_Location = NewLocation - CurrentParentWorld;
	}

	void WorldComponent::SetRelativeLocation(Vector NewLocation)
	{
		m_RelativeTransform.m_Location = NewLocation;
	}

	Rotator WorldComponent::GetWorldRotation()
	{
		WorldComponent* Parent = GetParentComponent();
		return Parent ? Parent->GetWorldRotation() + m_RelativeTransform.m_Rotation : m_RelativeTransform.m_Rotation;
	}

	Rotator WorldComponent::GetRelativeRotation()
	{
		return m_RelativeTransform.m_Rotation;
	}

	void WorldComponent::SetWorldRotation(Rotator NewRotation)
	{
		Rotator CurrentParentWorld;

		while (WorldComponent* Parent = GetParentComponent())
		{
			CurrentParentWorld += Parent->m_RelativeTransform.m_Rotation;
		}

		m_RelativeTransform.m_Rotation = NewRotation - CurrentParentWorld;
	}

	void WorldComponent::SetRelativeRotation(Rotator NewRotation)
	{
		m_RelativeTransform.m_Rotation = NewRotation;
	}

	Vector WorldComponent::GetWorldScale()
	{
		WorldComponent* Parent = GetParentComponent();
		return Parent ? Parent->GetWorldScale() + m_RelativeTransform.m_Scale : m_RelativeTransform.m_Scale;
	}

	Vector WorldComponent::GetRelativeScale()
	{
		return m_RelativeTransform.m_Scale;
	}

	void WorldComponent::SetWorldScale(Vector NewScale)
	{
		Vector CurrentParentWorld;

		while (WorldComponent* Parent = GetParentComponent())
		{
			CurrentParentWorld += Parent->m_RelativeTransform.m_Scale;
		}

		m_RelativeTransform.m_Scale = NewScale - CurrentParentWorld;
	}

	void WorldComponent::SetRelativeScale(Vector NewScale)
	{
		m_RelativeTransform.m_Scale = NewScale;
	}

	Transform WorldComponent::GetWorldTransform()
	{
		return Transform(GetWorldLocation(), GetWorldRotation(), GetWorldScale());
	}

	Transform WorldComponent::GetRelativeTransform()
	{
		return m_RelativeTransform;
	}

	void WorldComponent::SetWorldTransform(Transform NewTransform)
	{
		SetWorldLocation(NewTransform.m_Location);
		SetWorldRotation(NewTransform.m_Rotation);
		SetWorldScale(NewTransform.m_Scale);
	}

	void WorldComponent::SetRelativeTransform(Transform NewTransform)
	{
		m_RelativeTransform = NewTransform;
	}

	void WorldComponent::InitialisePhysics()
	{
	}

	btRigidBody* WorldComponent::GetRigidBody()
	{
		return nullptr;
	}

	void WorldComponent::Render()
	{
		for (auto Child : m_Children)
		{
			if (WorldComponent* WorldChild = static_cast<WorldComponent*>(Child))
				WorldChild->Render();
		}
	}

	Vector* WorldComponent::GetWriteablePosition()
	{
		return &m_RelativeTransform.m_Location;
	}
	Rotator* WorldComponent::GetWriteableRotation()
	{
		return &m_RelativeTransform.m_Rotation;
	}
	Vector* WorldComponent::GetWriteableScale()
	{
		return &m_RelativeTransform.m_Scale;
	}
	std::vector<Component*> WorldComponent::GetChildren() const
	{
		return m_Children;
	}
}


