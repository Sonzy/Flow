#include "Flowpch.h"
#include "Toolbar.h"
#include "ThirdParty/ImGui/imgui.h"
#include "Flow/Application.h"
#include "Flow/Layers/EditorLayer.h"
#include "Flow/GameFramework/World.h"
#include "ThirdParty\Bullet\LinearMath\btIDebugDraw.h"

Toolbar::Toolbar(EditorLayer* EditorPointer)
	: _Editor(EditorPointer)
{
}

ImVec2 Toolbar::Draw()
{
#if WITH_EDITOR
	ImVec2 MenuSize;
	if (ImGui::BeginMainMenuBar())
	{
		MenuSize = ImGui::GetWindowSize();


		//= FILE ================

		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Save (WIP)"))
				Application::SaveLevel();

			if (ImGui::MenuItem("Load (WIP)"))
				Application::LoadLevel();

			if (ImGui::MenuItem("Quit"))
				Application::Shutdown();

			ImGui::EndMenu();
		}


		//= TOOLS ================

		if (ImGui::BeginMenu("Tools"))
		{
			if (ImGui::MenuItem("Toggle ImGui Demo Window"))
				_Editor->ToggleImGuiDemoWindow();

			if (ImGui::MenuItem("Toggle Editor"))
			{
				Application& App = Application::GetApplication();
				App._DrawEditor = !App._DrawEditor;
			}

			if (ImGui::MenuItem("Create Test Window (UNSTABLE)"))
			{
				Application::CreateNewWindow("Test Window");
			}

			if (ImGui::MenuItem("Collision Editor (UNSTABLE)"))
			{
				/* Still need to decide how to handle multiple windows. Main window flickers since the subwindows
				call the render commands on the main window. EZ fix but havent decided how to lay out framework
				for drawing my own stuff to each window so ill come back to it another time*/
				_Editor->OpenCollisionEditor();
			}

			ImGui::EndMenu();
		}

		//= Options ================

		if (ImGui::BeginMenu("Options"))
		{
			if (ImGui::BeginMenu("Bullet Physics"))
			{
				if (ImGui::MenuItem("Configure Debug Colours"))
				{
					_ShowBulletConfigurationWindow = true;
				}

				ImGui::EndMenu();
			}

			ImGui::EndMenu();
		}

		//= Game ====================

		if (ImGui::BeginMenu("Game"))
		{
			if (ImGui::MenuItem("Play"))
				Application::StartGame();
			if (ImGui::MenuItem("Pause"))
				Application::PauseGame();
			if (ImGui::MenuItem("Stop (WIP)"))
				Application::StopGame();

			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}


	if (_ShowBulletConfigurationWindow)
		DrawBulletColourConfig();

	return MenuSize;
#endif

	return ImVec2(0, 0);
}

void Toolbar::DrawBulletColourConfig()
{
	if (ImGui::Begin("Bullet Physics Debug Colours", &_ShowBulletConfigurationWindow))
	{
		btIDebugDraw::DefaultColors& Colours = Application::GetWorld()->GetPhysicsDebugDrawer().GetDebugColours();
		ImGui::ColorPicker3("Active Objects", Colours.m_activeObject.m_floats);
		ImGui::ColorPicker3("Deactivated Objects", Colours.m_deactivatedObject.m_floats);
		ImGui::ColorPicker3("AABBs", Colours.m_aabb.m_floats);
		ImGui::ColorPicker3("Contact Points", Colours.m_contactPoint.m_floats);
		ImGui::ColorPicker3("Disabled Deactivation Objects", Colours.m_disabledDeactivationObject.m_floats);
		ImGui::ColorPicker3("Disabled Simulation Objects", Colours.m_disabledSimulationObject.m_floats);
		ImGui::ColorPicker3("Wants Deactivation Objects", Colours.m_wantsDeactivationObject.m_floats);
	}
	ImGui::End();
}
