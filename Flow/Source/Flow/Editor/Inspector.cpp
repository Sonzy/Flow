#include "Flowpch.h"
#include "Inspector.h"
#include "ThirdParty\ImGui\imgui.h"
#include "ThirdParty\ImGui\misc\cpp\imgui_stdlib.h"
#include "Flow\GameFramework\World.h"

#include "Flow\GameFramework\Actor.h"
#include "Flow\Editor\SelectionGizmo.h"

#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"

#include "Flow\Rendering\RenderCommand.h"
#include "Flow\Input\Input.h"

#include "Flow\Application.h"
#include "Flow\GameFramework\World.h"
#include "btBulletDynamicsCommon.h"

#include "Flow\GameFramework\Components\WorldComponent.h"
#include "Flow\GameFramework\Components\StaticMeshComponent.h" //TODO: Remove dependency



#include "Flow\GameFramework\Components\CameraComponent.h"
#include "Flow/GameFramework/Components/WorldComponent.h"

#include "Flow/Layers/EditorLayer.h"


Inspector::Inspector(SelectionGizmo* Selector)
	: _CurrentWorld(nullptr), _FocusedItem(nullptr), _Selector(Selector)
{
}

void Inspector::BeginPlay()
{
	_Selector->InitialisePhysics();
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

			if (!_HideTree && _FocusedItem->GetRootComponent())
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
				//TODO: Not sure what the difference is
				_FocusedComponent->DrawComponentDetailsWindow(); //TODO: Move this where I want
				_FocusedComponent->DrawDetailsWindow(false);
			}
		}

	}
	ImGui::End();
}

void Inspector::RenderHeirarchy()
{
	if (ImGui::Begin("Scene Heirarchy"))
	{
		ImGui::Text(std::string("Level: " + _CurrentWorld->GetName()).c_str());

		ImGui::Separator();

		for (auto Object : _CurrentWorld->GetActors())
		{
			bool NodeOpen = ImGui::TreeNode(Object->GetName().c_str());

			if (ImGui::IsItemClicked())
			{
				_FocusedItem = Object.get();
				_FocusedItemChanged = true;
				_FocusedComponent = _FocusedItem->GetRootComponent();

				//TODO: Make select obj function
				if (!_Selector->IsVisible())
				{
					_Selector->AddCollidersToWorld(World::GetWorld());
					_Selector->SetVisibility(true);
				}
				_Selector->OnNewComponentSelected(Object->GetRootComponent());
				_Selector->UpdatePosition(Object->GetLocation());
			}

			if(NodeOpen)
				ImGui::TreePop();
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
	Actor* HitObject = Ray.hasHit() ? HitComp->GetParentActor() : nullptr;

	if (_FocusedItem && HitObject != _FocusedItem)
	{
		if (SelectionGizmo* Gizmo = dynamic_cast<SelectionGizmo*>(HitObject))
		{
			SelectedAxis Axis = SelectedAxis::None;

			if(HitComp->_Tag._Equal("ArrowX"))
				Axis = SelectedAxis::X;
			if (HitComp->_Tag._Equal("ArrowY"))
				Axis = SelectedAxis::Y;
			if (HitComp->_Tag._Equal("ArrowZ"))
				Axis = SelectedAxis::Z;

			if (Axis == SelectedAxis::None)
			{
				FLOW_ENGINE_ERROR("Inspector::OnMouseClicked: Selected Selection Gizmo but failed to identify mesh");
				return true;
			}

			Gizmo->OnSelected(Axis, _FocusedComponent);
			return true;
		}
		else if (StaticMeshComponent* Comp = dynamic_cast<StaticMeshComponent*>(HitComp))
			Comp->EnableOutlineDrawing(false);

	}

	_FocusedItemChanged = HitObject && _FocusedItem != HitObject;
	_FocusedComponentChanged = HitComp && _FocusedComponent != HitComp;


	_FocusedItem = HitObject;

	if (_FocusedComponentChanged)
	{
		//If we had a selected component, update it before deselecting
		if (_FocusedComponent)
		{
			_FocusedComponent->OnViewportDeselected();
		}

		_FocusedComponent = HitComp;

		//If we have selected a new component
		if (_FocusedComponent)
		{
			_FocusedComponent->OnViewportSelected();
		}
	}



	//If we have not hit anything, reset the selector
	if (!_FocusedItem)
	{
		if(_FocusedComponent)
			_FocusedComponent->OnViewportDeselected();
		_FocusedComponent = nullptr;

		_Selector->SetScale(Vector(1.0f, 1.0f, 1.0f));
		_Selector->OnDeselected();
		_Selector->OnNewComponentSelected(nullptr);

		if (_Selector->IsVisible())
		{
			_Selector->RemoveCollidersFromWorld(World::GetWorld());
			_Selector->SetVisibility(false);
		}

	}
	else
	{
		if (!_Selector->IsVisible())
		{
			_Selector->AddCollidersToWorld(World::GetWorld());
			_Selector->SetVisibility(true);
		}
		_Selector->OnNewComponentSelected(HitComp);
		_Selector->UpdatePosition(HitObject->GetLocation());

	}


	return true;
}

bool Inspector::OnMouseReleased(MouseButtonReleasedEvent& e)
{
	if (_Selector->GetSelectedAxis() != SelectedAxis::None)
	{
		_Selector->OnDeselected();
		return true;
	}

	return false;
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
	_Selector->UpdateSelection();

	_FocusedItemChanged = false;
}

void Inspector::Render()
{
	RenderInspector();
	RenderHeirarchy();
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