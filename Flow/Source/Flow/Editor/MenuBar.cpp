#include "Flowpch.h"
#include "MenuBar.h"
#include "ThirdParty/ImGui/imgui.h"
#include "Flow/Application.h"
#include "Flow/Editor/EditorLayer.h"
#include "Flow/GameFramework/World.h"
#include "ThirdParty\Bullet\LinearMath\btIDebugDraw.h"

#include "Flow/Editor/Tools/SelectionTool.h"

MenuBar::MenuBar(EditorLayer* EditorPointer)
	: _Editor(EditorPointer)
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
				_Editor->Open_NewLevelWindow();//TODO: Open new levels

			if (ImGui::MenuItem("Save"))
				Application::SaveLevel();

			if (ImGui::MenuItem("Load"))
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

			ImGui::EndMenu();
		}

		//= Options ================

		if (ImGui::BeginMenu("Options"))
		{
			if (ImGui::BeginMenu("Editor Settings"))
			{
				if (ImGui::MenuItem("Colour Settings"))
				{
					_Visible_EditorSettings = true;
				}

				if (ImGui::BeginMenu("Tools"))
				{
					if (ImGui::MenuItem("Selection Tool"))
					{
						if (SelectionTool* tool = EditorLayer::GetEditor()->GetTool<SelectionTool>())
						{
							tool->OpenConfigWindow();
						}						
					}


					ImGui::EndMenu();
				}

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Bullet Physics"))
			{
				if (ImGui::MenuItem("Configure Debug Colours"))
				{
					_Visible_BulletConfiguration = true;
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


	if (_Visible_BulletConfiguration)
		Window_BulletDebugDrawSettings();
	if (_Visible_EditorSettings)
		Window_EditorSettings();

	return MenuSize;
#endif

	return ImVec2(0, 0);
}

void MenuBar::Window_BulletDebugDrawSettings()
{
	if (ImGui::Begin("Bullet Physics Debug Colours", &_Visible_BulletConfiguration))
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

void MenuBar::Window_EditorSettings()
{
	if (ImGui::Begin("Editor Settings", &_Visible_EditorSettings))
	{ 
		ImGui::ColorPicker3("Selected Object Highlight Colour", reinterpret_cast<float*>(&EditorLayer::GetEditorSettings()._ObjectHighlightColour));
	}
	ImGui::End();
}
