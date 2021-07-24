#include "pch.h"

#if WITH_EDITOR

#include "Inspector.h"
#include "ThirdParty\ImGui\imgui.h"
#include "ThirdParty\ImGui\imgui_stdlib.h"
#include "GameFramework\World.h"

#include "GameFramework\Actor.h"

#include "Bullet/btBulletCollisionCommon.h"
#include "Bullet/btBulletDynamicsCommon.h"

#include "Rendering\Renderer.h"
#include "Framework/Input\Input.h"

#include "Application.h"
#include "GameFramework\World.h"

#include "GameFramework\Components\WorldComponent.h"
#include "GameFramework\Components\StaticMeshComponent.h"
#include "GameFramework\Components\CameraComponent.h"

#include "Framework/Events/KeyEvent.h"

#include "Editor/Editor.h"
#include "Editor/Tools/SelectionTool.h"
#include "Editor/Modules/Spawner.h"


Inspector::Inspector()
	: m_CurrentWorld(World::Get())
	, m_RenameActor(nullptr)
	, m_HideTree(false)
	, m_HideWholeComponentTree(false)
{
}

void Inspector::Update()
{
	m_SelectedComponent = m_editor->GetTool<SelectionTool>()->GetSelectedComponent();
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
		ImGui::Text("Guid: %lu", m_SelectedComponent->GetGuid());
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
		ImGui::Text(std::string("Level: " + m_CurrentWorld.GetName()).c_str());

		ImGui::Separator();

		if (ImGui::BeginChild("Hierarchy"))
		{
			char Buffer[256] = { '\0' };
			for (std::pair<FGuid, Actor*> Object : m_CurrentWorld.GetActorMap())
			{
				Actor* actor = Object.second;
				sprintf_s(Buffer, "%s (%s)", actor->GetName().c_str(), actor->GetStaticName());

				if (m_RenameActor != nullptr && actor == m_RenameActor)
				{	
					if (ImGui::InputText("Name", m_RenameBuffer, 128, ImGuiInputTextFlags_EnterReturnsTrue))
					{
						m_RenameActor->SetName(m_RenameBuffer);
						m_RenameActor = nullptr;
					}
				}
				else
				{
					ImGui::PushID(actor->GetGuid());
					const bool selected = m_SelectedComponent ? m_SelectedComponent->GetParentActor() == actor : false;
					if (ImGui::Selectable(Buffer, selected))
					{
						//If we have changed the selected component, then stop the renaming process
						if (actor->GetRootComponent() != m_SelectedComponent)
						{
							m_RenameActor = nullptr;
						}

						m_editor->GetTool<SelectionTool>()->SelectComponent(actor->GetRootComponent());
						ImGui::GetIO().WantCaptureKeyboard = false;
					}

					ImGui::PopID();
				}
			}

			if (ImGui::BeginPopupContextWindow())
			{
				Editor::Get().GetModule<Spawner>()->DrawActorSpawnContextWindow();
				ImGui::EndPopup();
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
			m_RenameActor = Parent;
			sprintf_s(m_RenameBuffer, "%s", m_RenameActor->GetName().c_str());
			return true;
		}
	}

	if (m_RenameActor != nullptr && e.GetKeyCode() == KEY_ESC)
	{
		m_RenameActor = nullptr;
		return true;
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

	bUpdate |= ImGui::InputFloat3("Position", (float*)m_SelectedComponent->GetWriteablePosition(), "%.3f", ImGuiInputTextFlags_EnterReturnsTrue);
	bUpdate |= ImGui::InputFloat3("Rotation", (float*)m_SelectedComponent->GetWriteableRotation(), "%.3f", ImGuiInputTextFlags_EnterReturnsTrue);
	if (ImGui::InputFloat3("Scale", (float*)m_SelectedComponent->GetWriteableScale(), "%.3f", ImGuiInputTextFlags_EnterReturnsTrue))
	{
		bUpdate = true;
		m_SelectedComponent->UpdateCollisionScale();
	}
}

#endif WITH_EDITOR