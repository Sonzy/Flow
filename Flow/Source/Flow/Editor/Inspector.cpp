#include "Flowpch.h"
#include "Inspector.h"
#include "ThirdParty\ImGui\imgui.h"
#include "ThirdParty\ImGui\misc\cpp\imgui_stdlib.h"
#include "Flow\GameFramework\World.h"
#include "Flow\GameFramework\WorldObject.h"

#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"

#include "Flow\Rendering\RenderCommand.h"
#include "Flow\Input\Input.h"

#include "Flow\Application.h"
#include "Flow\GameFramework\World.h"
#include "btBulletDynamicsCommon.h"

#include "Flow\GameFramework\Components\WorldComponent.h"
#include "Flow\GameFramework\Components\StaticMeshComponent.h" //TODO: Remove dependency

#include "Flow\Editor\SelectionGizmo.h"

#include "Flow\GameFramework\Components\CameraComponent.h"
#include "Flow/GameFramework/Components/WorldComponent.h"

#include "Flow/Layers/EditorLayer.h"


Inspector::Inspector(SelectionGizmo* Selector)
	: _CurrentWorld(nullptr), _FocusedItem(nullptr), _Selector(Selector)
{
}

void Inspector::RenderInspector()
{
	if (ImGui::Begin("Inspector"))
	{
		if (_FocusedItem)
		{
			ImGui::InputText("ObjectName", &_FocusedItem->GetWritableName());
			ImGui::TextColored(IMGUI_ORANGE, (std::string("Selected Component: ") + (_FocusedComponent ? _FocusedComponent->GetName() : std::string("None"))).c_str());

			ImGui::Separator();

			if (!_HideTree)
				_FocusedItem->GetRootComponent()->DrawInspectionTree(_FocusedComponent);

			if (ImGui::Button(_HideTree ? "Show Tree" : "Collapse Tree"))
			{
				_HideTree = !_HideTree;
			}

			ImGui::Separator(); //==========================================


			if (_FocusedComponent)
			{
				ImGui::TextColored(IMGUI_YELLOW, "Component Transform");
				ImGui::Separator();
				ImGui::Dummy(ImVec2(0.0f, 10.0f));
				DrawSelectedComponentTransform();
			}

			ImGui::Separator();

			_FocusedItem->DrawDetailsWindow(_FocusedItemChanged);

			ImGui::Separator();

			if (_FocusedComponent)
			{
				_FocusedComponent->DrawDetailsWindow(false);
			}
		}

	}
	ImGui::End();
}

void Inspector::RenderHeirarchy()
{
	if (ImGui::Begin("Heirarchy"))
	{
		ImGui::Text(std::string("Level: " + _CurrentWorld->GetName()).c_str());

		ImGui::Separator();

		for (auto Object : _CurrentWorld->_WorldObjects)
		{
			if (ImGui::Button(Object->GetName().c_str()))
			{
				if (_FocusedItem)
				{
					if (StaticMeshComponent* Comp = dynamic_cast<StaticMeshComponent*>(_FocusedItem->GetRootComponent()))
						Comp->EnableOutlineDrawing(false);
				}

				//TODO: Select object on click
				_FocusedItem = Object.get();
				_FocusedItemChanged = true;
				_FocusedComponent = _FocusedItem->GetRootComponent();

				if (StaticMeshComponent* Comp = dynamic_cast<StaticMeshComponent*>(_FocusedItem->GetRootComponent()))
					Comp->EnableOutlineDrawing(true);
			}
		}
	}
	ImGui::End();
}

void Inspector::SetCurrentWorld(World* WorldReference)
{
	_CurrentWorld = WorldReference;
}

bool Inspector::OnMouseClicked(MouseButtonPressedEvent& e)
{
	//Ensure mouse left.
	if (e.GetMouseButton() != FLOW_MOUSE_LEFT)
		return false;

	//Calculate the ray bounds
	DirectX::XMFLOAT3 Pos = RenderCommand::GetMainCamera()->GetCameraPosition().ToDXFloat3();
	IntVector2D MousePosition = Input::GetMousePosition();
	Vector Start = Vector(Pos.x, Pos.y, Pos.z);
	Vector Direction = RenderCommand::GetScreenToWorldDirectionVector(MousePosition.X, MousePosition.Y);
	Vector End = Start + (Direction * 1000.0f);

	//Raytrace into the world
	btCollisionWorld::ClosestRayResultCallback Ray = World::WorldTrace(Start, End);

	//If we hit something, if it was a world component, assign this to the focused item.
	WorldComponent* HitComp = Ray.hasHit() ? static_cast<WorldComponent*>(Ray.m_collisionObject->getUserPointer()) : nullptr;
	WorldObject* HitObject = Ray.hasHit() ? HitComp->GetParentWorldObject() : nullptr;

	_FocusedItemChanged = HitObject && _FocusedItem != HitObject;
	_FocusedComponentChanged = HitComp && _FocusedComponent != HitComp;

	if (_FocusedItem && HitObject != _FocusedItem)
	{
		if (StaticMeshComponent* Comp = dynamic_cast<StaticMeshComponent*>(HitComp))
			Comp->EnableOutlineDrawing(false);
	}

	_FocusedItem = HitObject;

	if (_FocusedComponentChanged)
		_FocusedComponent = HitComp;

	//TODO: If we hit, run selection gizmo logic.
	if (Ray.hasHit() && static_cast<SelectionGizmo*>(Ray.m_collisionObject->getUserPointer()))
	{
		//FLOW_ENGINE_LOG("We hit the selection. TODO: Detection on each arro");
		if (StaticMeshComponent* Comp = dynamic_cast<StaticMeshComponent*>(_FocusedComponent))
			Comp->EnableOutlineDrawing(true);
	}

	if (!_FocusedItem)
	{
		_FocusedComponent = nullptr;
		//Selector_->SetVisibility(false);
		//Selector_->SetScale(Vector(1.0f, 1.0f, 1.0f));
	}
	//If we have not hit anything, reset the selector
	else
	{
		//m_Selector->UpdatePosition(HitObject->GetLocation());
		//m_Selector->SetScale(HitObject->GetScale().X);

		//TODO: Re-enable when setting this back up
		//m_Selector->SetVisibility(true);
	}


	return true;
}

void Inspector::UpdateSelectedComponent(WorldComponent* NewComp)
{
	EditorLayer::GetEditor()->GetInspector()->_FocusedComponent = NewComp;
}

WorldComponent* Inspector::GetSelectedComponent()
{
	return EditorLayer::GetEditor()->GetInspector()->_FocusedComponent;
}

void Inspector::Update()
{
	RenderInspector();
	RenderHeirarchy();

	_FocusedItemChanged = false;
}

void Inspector::DrawSelectedComponentTransform()
{
	bool bUpdate = false;
	bUpdate |= ImGui::InputFloat3("Position", (float*)_FocusedComponent->GetWriteablePosition(), 1, ImGuiInputTextFlags_EnterReturnsTrue);
	bUpdate |= ImGui::InputFloat3("Rotation", (float*)_FocusedComponent->GetWriteableRotation(), 1, ImGuiInputTextFlags_EnterReturnsTrue);
	bUpdate |= ImGui::InputFloat3("Scale", (float*)_FocusedComponent->GetWriteableScale(), 1, ImGuiInputTextFlags_EnterReturnsTrue);

	//TODO:
	//if (bUpdate)
	//	_FocusedComponent->UpdatePhysicsBody(true);
}

SelectionGizmo* Inspector::GetSelector() const
{
	return _Selector;
}