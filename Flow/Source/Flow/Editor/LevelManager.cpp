#include "Flowpch.h"
#include "LevelManager.h"
#include "ThirdParty/ImGui/imgui.h"

LevelManager::LevelManager()
	: m_Draw_NewLevelWindow(false)
{
	m_NewLevelName.resize(32);
}

void LevelManager::Open_NewLevelWindow()
{
	m_Draw_NewLevelWindow = true;
}

void LevelManager::DrawWindows()
{
	if(m_Draw_NewLevelWindow) Draw_NewLevelWindow();
}

void LevelManager::Draw_NewLevelWindow()
{
	if (ImGui::Begin("Create New Level"))
	{
		ImGui::InputText("Level Name", m_NewLevelName.data(), 32);
		if (ImGui::Button("Create Level"))
			FLOW_ENGINE_LOG("TODO: Create new level.");
	}
	ImGui::End();
}
