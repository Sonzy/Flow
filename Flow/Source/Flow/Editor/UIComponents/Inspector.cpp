#include "Flowpch.h"
#include "Inspector.h"
#include "ThirdParty\ImGui\imgui.h"
#include "ThirdParty\ImGui\misc\cpp\imgui_stdlib.h"
#include "GameFramework\World.h"

#include "GameFramework\Actor.h"
#include "Editor\SelectionGizmo.h"

#include "Bullet/btBulletCollisionCommon.h"
#include "Bullet/btBulletDynamicsCommon.h"

#include "Rendering\RenderCommand.h"
#include "Input\Input.h"

#include "Application.h"
#include "GameFramework\World.h"

#include "GameFramework\Components\WorldComponent.h"
#include "GameFramework\Components\StaticMeshComponent.h"
#include "GameFramework\Components\CameraComponent.h"

#include "Events/KeyEvent.h"

#include "Editor/Editor.h"
#include "Editor/Tools/SelectionTool.h"


Inspector::Inspector()
	: m_CurrentWorld(nullptr)
	, m_Renaming(false)
	, m_HideTree(false)
	, m_HideWholeComponentTree(false)
{
	m_CurrentWorld = World::Get();
}

void Inspector::Update()
{
	m_SelectedComponent = m_Editor->GetTool<SelectionTool>()->GetSelectedComponent();
}

void Inspector::Render()
{
	RenderInspector();
	RenderHeirarchy();
}

void Inspector::RenderInspector()
{
	if (ImGui::Begin("Inspector"))
	{
		//Context menu
		if (ImGui::BeginPopupContextWindow("ComponentDetailsContextMenu"))
		{
			ImGui::Selectable("Toggle Tree Visibility", &m_HideTree);

			ImGui::EndPopup();
		}

		if (m_SelectedComponent == nullptr)
		{
			ImGui::End();
			return;
		}

		Actor* ParentActor = m_SelectedComponent->GetParentActor();

		ImGui::InputText("ObjectName", &ParentActor->GetWritableName());
		ImGui::TextColored(IMGUI_ORANGE, (std::string("Selected Component: ") + (m_SelectedComponent == nullptr ? m_SelectedComponent->GetName() : std::string("None"))).c_str());

		if (m_HideTree == false && ParentActor->GetRootComponent())
		{
			ImGui::Separator();

			ParentActor->DrawInspectionTree(m_SelectedComponent);
		}

		ImGui::Separator(); //==========================================

		if (m_SelectedComponent)
		{
			ImGui::TextColored(IMGUI_YELLOW, "Component Transform");
			ImGui::Separator();
			ImGui::Dummy(ImVec2(0.0f, 10.0f));
			DrawSelectedComponentTransform();
		}

		ImGui::Separator();

		//_FocusedItem->DrawDetailsWindow(_FocusedItemChanged); //TODO: update focused item changed
		ParentActor->DrawDetailsWindow(false);

		ImGui::Separator();

		if (m_SelectedComponent)
		{
			//TODO: Not sure what the difference is
			m_SelectedComponent->DrawComponentDetailsWindow(); //TODO: Move this where I want
			m_SelectedComponent->DrawDetailsWindow(false);
		}
	}
	ImGui::End();
}

void Inspector::RenderHeirarchy()
{
	if (ImGui::Begin("Scene Hierarchy"))
	{
		ImGui::Text(std::string("Level: " + m_CurrentWorld->GetName()).c_str());

		ImGui::Separator();

		if (ImGui::BeginChild("Hierarchy"))
		{
			char Buffer[256] = { '\0' };
			int Counter = 0;
			for (std::pair<FGUID, Actor*> Object : m_CurrentWorld->GetActorMap())
			{
				Actor* actor = Object.second;
				sprintf_s(Buffer, "%s###%d", actor->GetName().c_str(), Counter);

				if (m_Renaming == false)
				{
					const bool selected = m_SelectedComponent ? m_SelectedComponent->GetParentActor() == actor : false;
					if (ImGui::Selectable(Buffer, selected))
					{
						m_Editor->GetTool<SelectionTool>()->SelectComponent(actor->GetRootComponent());
						ImGui::GetIO().WantCaptureKeyboard = false;
					}
				}
				else
				{					
					ImGui::InputText("Name", &m_SelectedComponent->GetParentActor()->GetWritableName());
				}


				Counter++;
			}
		}
		ImGui::EndChild();
	}
	ImGui::End();
}

bool Inspector::OnKeyPressed(KeyPressedEvent& e)
{
	//TODO: need to figure out how to pass some events from imgui
	if (m_SelectedComponent != nullptr && e.GetKeyCode() == KEY_F2)
	{
		if (Actor* Parent = m_SelectedComponent->GetParentActor())
		{
			//Start rename dialogue
			m_Renaming = true;
		}
	}
	return false;
}

WorldComponent* Inspector::GetSelectedComponent()
{
	return m_SelectedComponent;
}

void Inspector::DrawSelectedComponentTransform()
{
	bool bUpdate = false;
	RenderableComponent* rComp = dynamic_cast<RenderableComponent*>(m_SelectedComponent);

	if (rComp && rComp->UsingMatrixRotation())
	{
		float position[3];
		float rotation[3];
		float scale[3];
		DirectX::XMFLOAT4X4 fMatrix;
		DirectX::XMStoreFloat4x4(&fMatrix, rComp->GetMatrix());
		ImGuizmo::DecomposeMatrixToComponents((float*)&fMatrix, position, rotation, scale);

		//{
		//	Vector3 pos = rComp->GetWorldPosition();
		//	Rotator rot = rComp->GetWorldRotation();
		//	Vector3 sc = rComp->GetWorldScale();
		//
		//	FLOW_ENGINE_LOG("DIFFERENT Pos (Stored: %f %f %f) (Matrix: %f %f %f)", pos.x, pos.y, pos.z, position[0], position[1], position[2]);
		//	FLOW_ENGINE_LOG("DIFFERENT Rot (Stored: %f %f %f) (Matrix: %f %f %f)", rot.Pitch, rot.Roll, rot.Yaw, rotation[0], rotation[1], rotation[2]);
		//	FLOW_ENGINE_LOG("DIFFERENT Scale (Stored: %f %f %f) (Matrix: %f %f %f)", sc.x, sc.y, sc.z, scale[0], scale[1], scale[2]);
		//}


		//if (Vector3(position[0], position[1], position[2]) != rComp->GetWorldPosition())
		//{
		//	Vector3 pos = rComp->GetWorldPosition();
		//	FLOW_ENGINE_LOG("DIFFERENT Pos (Stored: %f %f %f) (Matrix: %f %f %f)", pos.x, pos.y, pos.z, position[0], position[1], position[2]);
		//}
		//
		//if (Rotator(rotation[0], rotation[1], rotation[2]) != rComp->GetWorldRotation())
		//{
		//	Rotator rot = rComp->GetWorldRotation();
		//	FLOW_ENGINE_LOG("DIFFERENT Rot (Stored: %f %f %f) (Matrix: %f %f %f)", rot.Pitch, rot.Roll, rot.Yaw, rotation[0], rotation[1], rotation[2]);
		//}
		//
		//if (Vector3(scale[0], scale[1], scale[2]) != rComp->GetWorldScale())
		//{
		//	Vector3 sc = rComp->GetWorldScale();
		//	FLOW_ENGINE_LOG("DIFFERENT Scale (Stored: %f %f %f) (Matrix: %f %f %f)", sc.x, sc.y, sc.z, scale[0], scale[1], scale[2]);
		//}

		bUpdate |= ImGui::InputFloat3("Position", position, 1, ImGuiInputTextFlags_EnterReturnsTrue);
		bUpdate |= ImGui::InputFloat3("Rotation", rotation, 1, ImGuiInputTextFlags_EnterReturnsTrue);
		bUpdate |= ImGui::InputFloat3("Scale", scale, 1, ImGuiInputTextFlags_EnterReturnsTrue);

		if (bUpdate == true)
		{
			Rotator RadianRotation = Rotator::AsRadians(*(Rotator*)&rotation);

			DirectX::XMFLOAT4X4 fNewMatrix;
			DirectX::XMMATRIX matrix = 	DirectX::XMMatrixScaling(scale[0], scale[1], scale[2]) *
				DirectX::XMMatrixRotationRollPitchYaw(RadianRotation.Pitch, RadianRotation.Yaw, RadianRotation.Roll) *
				DirectX::XMMatrixTranslation(position[0], position[1], position[2]);

			DirectX::XMStoreFloat4x4(&fNewMatrix, matrix);

			Maths::PrintMatrix(matrix);
			FLOW_ENGINE_LOG("...															   ...");
			Maths::PrintMatrix(rComp->GetMatrix());

			FLOW_ENGINE_LOG("---------------------------------------------------------------------");

			rComp->SetMatrix(fNewMatrix);
		}
	}
	else
	{
		bUpdate |= ImGui::InputFloat3("Position", (float*)m_SelectedComponent->GetWriteablePosition(), 1, ImGuiInputTextFlags_EnterReturnsTrue);
		bUpdate |= ImGui::InputFloat3("Rotation", (float*)m_SelectedComponent->GetWriteableRotation(), 1, ImGuiInputTextFlags_EnterReturnsTrue);
		if (ImGui::InputFloat3("Scale", (float*)m_SelectedComponent->GetWriteableScale(), 1, ImGuiInputTextFlags_EnterReturnsTrue))
		{
			bUpdate = true;
			m_SelectedComponent->UpdateCollisionScale();
		}
	}


}