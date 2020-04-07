#include "Flowpch.h"
#include "WorldComponent.h"
#include "Flow\GameFramework\WorldObject.h"

namespace Flow
{
	WorldComponent::WorldComponent()
		: Component("Unnamed WorldComponent"), ParentComponent_(nullptr)
	{
	}

	WorldComponent::WorldComponent(const std::string& Name)
		: Component(Name), ParentComponent_(nullptr)
	{
	}

	WorldComponent::~WorldComponent()
	{
		Children_.clear();
	}

	void WorldComponent::Tick(float DeltaTime)
	{
		for (auto& Child : Children_)
		{
			Child->Tick(DeltaTime);
		}
	}

	void WorldComponent::AddChild(WorldComponent* Child)
	{
		CHECK_RETURN(!Child, "WorldComponent::AddChild: New Child was nullptr");
		Children_.push_back(Child);
		Child->SetParentComponent(this);
	}

	Vector WorldComponent::GetWorldPosition()
	{
		WorldComponent* Parent = GetParentComponent(); //TODO: Rotate this by the parents rotation
		return Parent ? Parent->GetWorldPosition() + RelativeTransform_.Position_ : RelativeTransform_.Position_;
	}

	Vector WorldComponent::GetRelativePosition()
	{
		return RelativeTransform_.Position_;
	}

	void WorldComponent::SetWorldPosition(Vector NewPosition)
	{
		Vector CurrentParentWorld;
		
		while (WorldComponent* Parent = GetParentComponent())
		{
			CurrentParentWorld += Parent->RelativeTransform_.Position_;
		}

		RelativeTransform_.Position_ = NewPosition - CurrentParentWorld;
	}

	void WorldComponent::SetRelativePosition(Vector NewPosition)
	{
		RelativeTransform_.Position_ = NewPosition;
	}

	Rotator WorldComponent::GetWorldRotation()
	{
		WorldComponent* Parent = GetParentComponent();
		return Parent ? Parent->GetWorldRotation() + RelativeTransform_.Rotation_ : RelativeTransform_.Rotation_;
	}

	Rotator WorldComponent::GetRelativeRotation()
	{
		return RelativeTransform_.Rotation_;
	}

	void WorldComponent::SetWorldRotation(Rotator NewRotation)
	{
		Rotator CurrentParentWorld;

		while (WorldComponent* Parent = GetParentComponent())
		{
			CurrentParentWorld += Parent->RelativeTransform_.Rotation_;
		}

		RelativeTransform_.Rotation_ = NewRotation - CurrentParentWorld;

		//TODO: UpdatePhysics Movement
	}

	void WorldComponent::SetRelativeRotation(Rotator NewRotation)
	{
		RelativeTransform_.Rotation_ = NewRotation;
		//TODO: UpdatePhysics Movement
	}

	Vector WorldComponent::GetWorldScale()
	{
		WorldComponent* Parent = GetParentComponent();
		return Parent ? Parent->GetWorldScale() + RelativeTransform_.Scale_ : RelativeTransform_.Scale_;
	}

	Vector WorldComponent::GetRelativeScale()
	{
		return RelativeTransform_.Scale_;
	}

	void WorldComponent::SetWorldScale(Vector NewScale)
	{
		Vector CurrentParentWorld;

		while (WorldComponent* Parent = GetParentComponent())
		{
			CurrentParentWorld += Parent->RelativeTransform_.Scale_;
		}

		RelativeTransform_.Scale_ = NewScale - CurrentParentWorld;
	}

	void WorldComponent::SetRelativeScale(Vector NewScale)
	{
		RelativeTransform_.Scale_ = NewScale;
	}

	Transform WorldComponent::GetWorldTransform()
	{
		return Transform(GetWorldPosition(), GetWorldRotation(), GetWorldScale());
	}

	Transform WorldComponent::GetRelativeTransform()
	{
		return RelativeTransform_;
	}

	void WorldComponent::SetWorldTransform(Transform NewTransform)
	{
		SetWorldPosition(NewTransform.Position_);
		SetWorldRotation(NewTransform.Rotation_);
		SetWorldScale(NewTransform.Scale_);
	}

	void WorldComponent::SetRelativeTransform(Transform NewTransform)
	{
		RelativeTransform_ = NewTransform;
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
		for (auto Child : Children_)
		{
			if (WorldComponent* WorldChild = static_cast<WorldComponent*>(Child))
				WorldChild->Render();
		}
	}

	Vector* WorldComponent::GetWriteablePosition()
	{
		return &RelativeTransform_.Position_;
	}
	Rotator* WorldComponent::GetWriteableRotation()
	{
		return &RelativeTransform_.Rotation_;
	}
	Vector* WorldComponent::GetWriteableScale()
	{
		return &RelativeTransform_.Scale_;
	}
	std::vector<Component*> WorldComponent::GetChildren() const
	{
		return Children_;
	}
	void WorldComponent::SetVisibility(bool Visible)
	{
		Visible_ = Visible;
	}
	bool WorldComponent::IsVisible() const
	{
		return Visible_;
	}
}


