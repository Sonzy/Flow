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
#include "Flow\GameFramework\Components\StaticMeshComponent.h"

#include "Flow\Editor\SelectionGizmo.h"

#include "Flow\GameFramework\Components\CameraComponent.h"


namespace Flow
{
	Inspector::Inspector(SelectionGizmo* Selector)
		: CurrentWorld_(nullptr), FocusedItem_(nullptr), Selector_(Selector)
	{
	}

	void Inspector::RenderInspector()
	{
		if (ImGui::Begin("Inspector"))
		{
			if (FocusedItem_)
			{
				ImGui::InputText("ObjectName", &FocusedItem_->GetNameNonConst());
				ImGui::TextColored(IMGUI_ORANGE,(std::string("Selected Component: ") + (FocusedComponent_ ? FocusedComponent_->GetName() : std::string("None"))).c_str());

				ImGui::Separator();
				
				if(!HideTree)
					FocusedItem_->GetRootComponent()->DrawInspectionTree(FocusedComponent_);

				if (ImGui::Button(HideTree ? "Show Tree" : "Collapse Tree"))
				{
					HideTree = !HideTree;
				}
					
				ImGui::Separator(); //==========================================


				if (FocusedComponent_)
				{
					ImGui::TextColored(IMGUI_YELLOW, "Component Transform");
					ImGui::Separator();
					ImGui::Dummy(ImVec2(0.0f, 10.0f));
					DrawSelectedComponentTransform();
				}

				ImGui::Separator();

				FocusedItem_->DrawDetailsWindow(FocusedItemChanged);

				ImGui::Separator();

				if (FocusedComponent_)
				{
					FocusedComponent_->DrawDetailsWindow(false);
				}
			}

		}
		ImGui::End();
	}

	void Inspector::RenderHeirarchy()
	{
		if (ImGui::Begin("Heirarchy"))
		{
			ImGui::Text(std::string("Level: " + CurrentWorld_->GetName()).c_str());

			ImGui::Separator();

			for (auto Object : CurrentWorld_->WorldObjects_)
			{
				if (ImGui::Button(Object->GetName().c_str()))
				{
					if (FocusedItem_)
					{
						if (StaticMeshComponent* Comp = dynamic_cast<StaticMeshComponent*>(FocusedItem_->GetRootComponent()))
							Comp->EnableOutlineDrawing(false);
					}

					//TODO: Select object on click
					FocusedItem_ = Object.get();
					FocusedItemChanged = true;
					FocusedComponent_ = FocusedItem_->GetRootComponent();

					if (StaticMeshComponent* Comp = dynamic_cast<StaticMeshComponent*>(FocusedItem_->GetRootComponent()))
						Comp->EnableOutlineDrawing(true);
				}
			}
		}
		ImGui::End();
	}

	void Inspector::SetCurrentWorld(World* WorldReference)
	{
		CurrentWorld_ = WorldReference;
	}

	bool Inspector::OnMouseClicked(MouseButtonPressedEvent& e)
	{
		//Ensure mouse left.
		if (e.GetMouseButton() != FLOW_MOUSE_LEFT)
			return false;

		//Calculate the ray bounds
		DirectX::XMFLOAT3 Pos = RenderCommand::GetCamera().GetCameraPosition().ToDXFloat3();
		IntVector2D MousePosition = Input::GetMousePosition();
		Vector Start = Vector(Pos.x, Pos.y, Pos.z);
		Vector Direction = RenderCommand::GetScreenToWorldDirectionVector(MousePosition.X, MousePosition.Y);
		Vector End = Start + (Direction * 1000.0f);

		//Raytrace into the world
		btCollisionWorld::ClosestRayResultCallback Ray = World::WorldTrace(Start, End);

		//If we hit something, if it was a world component, assign this to the focused item.
		WorldComponent* HitComp = Ray.hasHit() ? static_cast<WorldComponent*>(Ray.m_collisionObject->getUserPointer()) : nullptr;
		WorldObject* HitObject = Ray.hasHit() ? HitComp->GetParentWorldObject() : nullptr;

		FocusedItemChanged = HitObject && FocusedItem_ != HitObject;
		FocusedComponentChanged = HitComp && FocusedComponent_ != HitComp;

		if (FocusedItem_ && HitObject != FocusedItem_)
		{
			if (StaticMeshComponent* Comp = dynamic_cast<StaticMeshComponent*>(HitComp))
				Comp->EnableOutlineDrawing(false);
		}

		FocusedItem_ = HitObject;

		if (FocusedComponentChanged)
			FocusedComponent_ = HitComp;

		//TODO: If we hit, run selection gizmo logic.
		if (Ray.hasHit() && static_cast<SelectionGizmo*>(Ray.m_collisionObject->getUserPointer()))
		{
			//FLOW_ENGINE_LOG("We hit the selection. TODO: Detection on each arro");
			if(StaticMeshComponent* Comp = dynamic_cast<StaticMeshComponent*>(FocusedComponent_))
				Comp->EnableOutlineDrawing(true);
		}

		if (!FocusedItem_)
		{
			FocusedComponent_ = nullptr;
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

	void Inspector::Update()
	{
		RenderInspector();
		RenderHeirarchy();

		FocusedItemChanged = false;
	}

	void Inspector::UpdateSelectedComponent(WorldComponent* NewComp)
	{
		Application::GetApplication().GetInspector()->FocusedComponent_ = NewComp;
	}

	WorldComponent* Inspector::GetSelectedComponent()
	{
		return Application::GetApplication().GetInspector()->FocusedComponent_;
	}

	SelectionGizmo* Inspector::GetSelector() const
	{
		return Selector_;
	}

	void Inspector::DrawSelectedComponentTransform()
	{
		bool bUpdate = false;
		bUpdate |= ImGui::InputFloat3("Position", (float*)FocusedComponent_->GetWriteablePosition(), 1, ImGuiInputTextFlags_EnterReturnsTrue);
		bUpdate |= ImGui::InputFloat3("Rotation", (float*)FocusedComponent_->GetWriteableRotation(), 1, ImGuiInputTextFlags_EnterReturnsTrue);
		bUpdate |= ImGui::InputFloat3("Scale", (float*)FocusedComponent_->GetWriteableScale(), 1, ImGuiInputTextFlags_EnterReturnsTrue);

		if (bUpdate)
			FocusedComponent_->UpdatePhysicsBody(true);
	}
}