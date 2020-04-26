#include "Flowpch.h"
#include "Component.h"
#include "ThirdParty\ImGui\imgui.h"

namespace Flow
{
	Component::Component()
		: Component("Unnamed Component")
	{
	}

	Component::Component(const std::string& Name)
		: GameObject(Name), ParentObject_(nullptr), AttachedComponent_(nullptr)
	{
	}

	Component::~Component()
	{
	}

	WorldComponent* Component::GetParentComponent() const
	{
		return AttachedComponent_;
	}

	WorldObject* Component::GetParentWorldObject() const
	{
		return ParentObject_;
	}

	void Component::SetParent(WorldObject* Parent)
	{
		ParentObject_ = Parent;
	}

	void Component::SetParentComponent(WorldComponent* Parent)
	{
		AttachedComponent_ = Parent;
	}

	void Component::Tick(float DeltaTime)
	{

	}

	void Component::DrawDetailsWindow(bool bDontUpdate)
	{
		ImGui::TextColored(IMGUI_YELLOW, "Component Properties");
		ImGui::Separator();
		ImGui::Dummy(ImVec2(0.0f, 10.0f));		
	}
	void Component::BeginPlay()
	{
		BegunPlay_ = true;
	}
}


