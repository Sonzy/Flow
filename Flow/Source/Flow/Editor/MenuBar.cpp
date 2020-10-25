#include "Flowpch.h"
#include "MenuBar.h"
#include "ThirdParty/ImGui/imgui.h"
#include "Application.h"
#include "Editor/Editor.h"
#include "GameFramework/World.h"
#include "Bullet\LinearMath\btIDebugDraw.h"

#include "Editor/Tools/SelectionTool.h"

//TODO: Remove
#include "Assets/AssetSystem.h"

MenuBar::MenuBar(Editor* EditorPointer)
	: m_Editor(EditorPointer)
{
}

ImVec2 MenuBar::Draw()
{
#if WITH_EDITOR
	ImVec2 MenuSize;
	if (ImGui::BeginMainMenuBar())
	{
		MenuSize = ImGui::GetWindowSize();


		//= FILE ================

		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("New Level"))
			{
				m_Editor->Open_NewLevelWindow();//TODO: Open new levels
			}

			if (ImGui::MenuItem("Save"))
			{
				Application::SaveLevel();
			}

			if (ImGui::MenuItem("Load"))
			{
				Application::LoadLevel();
			}

			if (ImGui::MenuItem("Quit"))
			{
				Application::Shutdown();
			}

			ImGui::EndMenu();
		}


		//= TOOLS ================

		if (ImGui::BeginMenu("Tools"))
		{
			if (ImGui::MenuItem("Toggle ImGui Demo Window"))
				m_Editor->ToggleImGuiDemoWindow();

			if (ImGui::MenuItem("Toggle Editor"))
			{
				Application& App = Application::Get();
				App.m_RenderEditor = !App.m_RenderEditor;
			}

			ImGui::EndMenu();
		}

		//= Options ================

		if (ImGui::BeginMenu("Options"))
		{
			if (ImGui::MenuItem("Editor Settings"))
			{
				Editor::GetEditor()->ShowSettingsWindow(true);

				if (ImGui::BeginMenu("Tools"))
				{
					if (ImGui::MenuItem("Selection Tool"))
					{
						if (SelectionTool* tool = Editor::GetEditor()->GetTool<SelectionTool>())
						{
							tool->OpenConfigWindow();
						}						
					}


					ImGui::EndMenu();
				}
			}

			if (ImGui::BeginMenu("Bullet Physics"))
			{
				//TODO: Move this to seperate file
				if (ImGui::MenuItem("Configure Debug Colours"))
				{
					m_Visible_BulletConfiguration = true;
				}

				if (ImGui::MenuItem("Log all debug drawing"))
				{
					BulletDebugDraw& drawer = Application::GetWorld()->GetPhysicsDebugDrawer();
					drawer.EnableDrawingLogging(!drawer.IsLoggingDrawing());
				}

				if (ImGui::MenuItem("Print physics state"))
				{
					Application::GetWorld()->PrintAllPhysicsObjects();
				}

				ImGui::EndMenu();
			}

			ImGui::EndMenu();
		}

		//= Game ====================

		if (ImGui::BeginMenu("Game"))
		{
			if (ImGui::MenuItem("Play"))
			{
				Application::SavePlayState();
				Application::StartGame();
			}
	
			if (ImGui::MenuItem("Pause"))
				Application::PauseGame();

			if (ImGui::MenuItem("Stop (WIP)"))
			{
				if(Application::StopGame())
					Application::LoadPlayState();
			}


			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}


	if (m_Visible_BulletConfiguration)
	{
		Window_BulletDebugDrawSettings();
	}

	return MenuSize;
#endif

	return ImVec2(0, 0);
}

void MenuBar::Window_BulletDebugDrawSettings()
{
	if (ImGui::Begin("Bullet Physics Debug Colours", &m_Visible_BulletConfiguration))
	{
		btIDebugDraw::DefaultColors& Colors = Application::GetWorld()->GetPhysicsDebugDrawer().GetDebugColors();
		ImGui::ColorEdit3("Active Objects", Colors.m_activeObject.m_floats);
		ImGui::ColorEdit3("Deactivated Objects", Colors.m_deactivatedObject.m_floats);
		ImGui::ColorEdit3("AABBs", Colors.m_aabb.m_floats);
		ImGui::ColorEdit3("Contact Points", Colors.m_contactPoint.m_floats);
		ImGui::ColorEdit3("Disabled Deactivation Objects", Colors.m_disabledDeactivationObject.m_floats);
		ImGui::ColorEdit3("Disabled Simulation Objects", Colors.m_disabledSimulationObject.m_floats);
		ImGui::ColorEdit3("Wants Deactivation Objects", Colors.m_wantsDeactivationObject.m_floats);
	}
	ImGui::End();
}