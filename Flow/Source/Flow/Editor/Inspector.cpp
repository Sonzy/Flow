#include "Flowpch.h"
#include "Inspector.h"
#include "ThirdParty\ImGui\imgui.h"
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
#include "Flow\GameFramework\Components\StaticMeshComponent.h"

#include "Flow\Editor\SelectionGizmo.h"

#include "Flow\GameFramework\Components\CameraComponent.h"


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
			_FocusedItem->DrawDetailsWindow(_FocusedItemChanged);

			ImGui::Separator(); //==========================================

			//TODO:
			ImGui::Text((std::string("Selected Component: ") + _FocusedItem->GetRootComponent()->GetName()).c_str());

			_FocusedItem->GetRootComponent()->DrawComponentDetailsWindow();
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
				//TODO: Select object on click
				_FocusedItem = Object.get();

				if (StaticMeshComponent* Comp = static_cast<StaticMeshComponent*>(_FocusedItem->GetRootComponent()))
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
	WorldObject* HitObject = Ray.hasHit() ? static_cast<WorldComponent*>(Ray.m_collisionObject->getUserPointer())->GetParentWorldObject() : nullptr;

	if (_FocusedItem && HitObject != _FocusedItem)
	{
		if (StaticMeshComponent* Comp = static_cast<StaticMeshComponent*>(_FocusedItem->GetRootComponent()))
			Comp->EnableOutlineDrawing(false);
	}

	_FocusedItem = HitObject;

	//TODO: If we hit, run selection gizmo logic.
	if (Ray.hasHit() && static_cast<SelectionGizmo*>(Ray.m_collisionObject->getUserPointer()))
	{
		//FLOW_ENGINE_LOG("We hit the selection. TODO: Detection on each arro");
		if (StaticMeshComponent* Comp = static_cast<StaticMeshComponent*>(_FocusedItem->GetRootComponent()))
			Comp->EnableOutlineDrawing(true);
	}

	if (!_FocusedItem)
	{
		//_Selector->SetVisibility(false);
		//_Selector->SetScale(Vector(1.0f, 1.0f, 1.0f));
	}
	//If we have not hit anything, reset the selector
	else
	{
		//m_Selector->UpdatePosition(HitObject->GetLocation());
		//m_Selector->SetScale(HitObject->GetScale().X);

		//TODO: Re-enable when setting this back up
		//m_Selector->SetVisibility(true);
	}

	_FocusedItemChanged = true;
	return true;
}

void Inspector::Update()
{
	RenderInspector();
	RenderHeirarchy();

	_FocusedItemChanged = false;
}

SelectionGizmo* Inspector::GetSelector() const
{
	return _Selector;
}