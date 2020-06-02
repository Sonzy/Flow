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
	ImVec2 MenuSize;
	if (ImGui::BeginMainMenuBar())
	{
		MenuSize = ImGui::GetWindowSize();


		//= FILE ================

		if (ImGui::BeginMenu("File"))
		{
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

		ImGui::EndMainMenuBar();
	}


	if (_ShowBulletConfigurationWindow)
		DrawBulletColourConfig();

	return MenuSize;
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
