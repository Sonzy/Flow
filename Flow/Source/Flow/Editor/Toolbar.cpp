#include "Flowpch.h"
#include "Toolbar.h"
#include "ThirdParty/ImGui/imgui.h"
#include "Flow/Application.h"
#include "Flow/Layers/EditorLayer.h"

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

		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Quit"))
				Application::Shutdown();

			ImGui::EndMenu();
		}

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

		ImGui::EndMainMenuBar();
	}

	return MenuSize;
}