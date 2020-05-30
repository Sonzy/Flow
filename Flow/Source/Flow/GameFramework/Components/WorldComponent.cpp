#include "Flowpch.h"
#include "WorldComponent.h"
#include "Flow\GameFramework\WorldObject.h"
#include "ThirdParty\ImGui\imgui.h"
#include "Flow\Editor\Inspector.h"

WorldComponent::WorldComponent()
	: Component("Unnamed WorldComponent"), _ParentComponent(nullptr)
{
}

WorldComponent::WorldComponent(const std::string& Name)
	: Component(Name), _ParentComponent(nullptr)
{
}

WorldComponent::~WorldComponent()
{
	_Children.clear();
}

void WorldComponent::Tick(float DeltaTime)
{
	PROFILE_FUNCTION();

	for (auto& Child : _Children)
	{
		Child->Tick(DeltaTime);
	}
}

void WorldComponent::AddChild(WorldComponent* Child)
{
	CHECK_RETURN(!Child, "WorldComponent::AddChild: New Child was nullptr");
	_Children.push_back(Child);
	Child->SetParentComponent(this);
}

Vector WorldComponent::GetWorldPosition() const
{
	WorldComponent* Parent = GetParentComponent(); //TODO: Rotate this by the parents rotation
	//return Parent ? Parent->GetWorldPosition() + _RelativeTransform._Position : _RelativeTransform._Position;

	if (Parent)
	{
		Rotator ParentRotation = Rotator::AsRadians(Parent->GetWorldRotation());

		DirectX::XMVECTOR Position = DirectX::XMLoadFloat3(&_RelativeTransform._Position.ToDXFloat3());
		DirectX::XMVECTOR Rotation = DirectX::XMQuaternionRotationRollPitchYaw(ParentRotation.Pitch, ParentRotation.Yaw, ParentRotation.Roll);
		DirectX::XMVECTOR NewPosition = DirectX::XMVector3Rotate(Position, Rotation);
		DirectX::XMFLOAT3 RotatedPos;
		DirectX::XMStoreFloat3(&RotatedPos, NewPosition);

		return Parent->GetWorldPosition() + Vector(RotatedPos.x, RotatedPos.y, RotatedPos.z);
	}
	else
		return _RelativeTransform._Position;
}

Vector WorldComponent::GetRelativePosition() const
{
	return _RelativeTransform._Position;
}

void WorldComponent::SetWorldPosition(Vector NewPosition)
{
	Vector CurrentParentWorld;
	WorldComponent* Parent = GetParentComponent();
	while (Parent)
	{
		CurrentParentWorld += Parent->_RelativeTransform._Position;
		Parent = Parent->GetParentComponent();
	}

	_RelativeTransform._Position = NewPosition - CurrentParentWorld;
}

void WorldComponent::SetRelativePosition(Vector NewPosition)
{
	_RelativeTransform._Position = NewPosition;
}

void WorldComponent::AddRelativePosition(Vector Position)
{
	_RelativeTransform._Position += Position;
}

Rotator WorldComponent::GetWorldRotation() const
{
	WorldComponent* Parent = GetParentComponent();
	return Parent ? Parent->GetWorldRotation() + _RelativeTransform._Rotation : _RelativeTransform._Rotation;
}

Rotator WorldComponent::GetRelativeRotation() const
{
	return _RelativeTransform._Rotation;
}

void WorldComponent::SetWorldRotation(Rotator NewRotation)
{
	Rotator CurrentParentWorld;

	if (WorldComponent* Parent = GetParentComponent())
		CurrentParentWorld = Parent->GetWorldRotation();

	_RelativeTransform._Rotation = NewRotation - CurrentParentWorld;

	//TODO: UpdatePhysics Movement
}

void WorldComponent::SetRelativeRotation(Rotator NewRotation)
{
	_RelativeTransform._Rotation = NewRotation;
	//TODO: UpdatePhysics Movement
}

void WorldComponent::AddRelativeRotation(Rotator Rotation)
{
	_RelativeTransform._Rotation += Rotation;
}

Vector WorldComponent::GetWorldScale() const
{
	WorldComponent* Parent = GetParentComponent();
	return Parent ? Parent->GetWorldScale() * _RelativeTransform._Scale : _RelativeTransform._Scale;
}

Vector WorldComponent::GetRelativeScale() const
{
	return _RelativeTransform._Scale;
}

void WorldComponent::SetWorldScale(Vector NewScale)
{
	Vector CurrentParentWorld;
	WorldComponent* PointedComp = GetParentComponent();
	while (PointedComp)
	{
		CurrentParentWorld *= PointedComp->_RelativeTransform._Scale;
		PointedComp = PointedComp->GetParentComponent();
	}

	_RelativeTransform._Scale = NewScale - CurrentParentWorld;
}

void WorldComponent::SetRelativeScale(Vector NewScale)
{
	_RelativeTransform._Scale = NewScale;
}

Transform WorldComponent::GetWorldTransform() const
{
	return Transform(GetWorldPosition(), GetWorldRotation(), GetWorldScale());
}

Transform WorldComponent::GetRelativeTransform() const
{
	return _RelativeTransform;
}

void WorldComponent::SetWorldTransform(Transform NewTransform)
{
	SetWorldPosition(NewTransform._Position);
	SetWorldRotation(NewTransform._Rotation);
	SetWorldScale(NewTransform._Scale);
}

void WorldComponent::SetRelativeTransform(Transform NewTransform)
{
	_RelativeTransform = NewTransform;
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
	for (auto Child : _Children)
	{
		if (WorldComponent* WorldChild = static_cast<WorldComponent*>(Child))
			WorldChild->Render();
	}
}

Vector* WorldComponent::GetWriteablePosition()
{
	return &_RelativeTransform._Position;
}
Rotator* WorldComponent::GetWriteableRotation()
{
	return &_RelativeTransform._Rotation;
}
Vector* WorldComponent::GetWriteableScale()
{
	return &_RelativeTransform._Scale;
}
void WorldComponent::DrawInspectionTree(WorldComponent* CurrentInspectedComponent, bool DontOpenTree)
{
	const ImVec2 ButtonSize = ImVec2(100, 12);
	const float RightOffset = 20;

	if (DontOpenTree)
		ImGui::SetNextItemOpen(false);

	bool NodeOpen = ImGui::TreeNode(GetName().c_str());

	if (ImGui::IsItemClicked())
		Inspector::UpdateSelectedComponent(this);

	if (!DontOpenTree && CurrentInspectedComponent == this)
	{
		ImGui::SameLine();
		ImGui::TextColored(IMGUI_GREEN, "Selected");
	}

	if (NodeOpen)
	{
		ImVec2 pos = ImGui::GetCursorScreenPos();
		ImU32 col = ImColor(ImGui::GetStyle().Colors[ImGuiCol_HeaderHovered]);

		for (auto& Child : _Children)
		{
			Child->DrawInspectionTree(CurrentInspectedComponent, DontOpenTree);
		}
		ImGui::TreePop();
	}

}
void WorldComponent::DrawDetailsWindow(bool bDontUpdate)
{
}
std::vector<WorldComponent*> WorldComponent::GetChildren() const
{
	return _Children;
}
void WorldComponent::SetVisibility(bool Visible)
{
	_Visible = Visible;
}
bool WorldComponent::IsVisible() const
{
	return _Visible;
}


