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
			for (Actor* Object : m_CurrentWorld->GetActors())
			{
				sprintf_s(Buffer, "%s###%d", Object->GetName().c_str(), Counter);

				if (m_Renaming == false)
				{
					const bool selected = m_SelectedComponent ? m_SelectedComponent->GetParentActor() == Object : false;
					if (ImGui::Selectable(Buffer, selected))
					{
						m_Editor->GetTool<SelectionTool>()->SelectComponent(Object->GetRootComponent());
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
	bUpdate |= ImGui::InputFloat3("Position", (float*)m_SelectedComponent->GetWriteablePosition(), 1, ImGuiInputTextFlags_EnterReturnsTrue);
	bUpdate |= ImGui::InputFloat3("Rotation", (float*)m_SelectedComponent->GetWriteableRotation(), 1, ImGuiInputTextFlags_EnterReturnsTrue);
	if (ImGui::InputFloat3("Scale", (float*)m_SelectedComponent->GetWriteableScale(), 1, ImGuiInputTextFlags_EnterReturnsTrue))
	{
		bUpdate = true;
		m_SelectedComponent->UpdateCollisionScale();
	}
}