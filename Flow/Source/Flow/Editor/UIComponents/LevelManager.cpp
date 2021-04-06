#include "pch.h"
#include "LevelManager.h"
#include "ThirdParty/ImGui/imgui.h"

LevelManager::LevelManager()
	: m_DrawWindow_NewLevel(false)
{
	m_NewLevelName.resize(32);
}

void LevelManager::Update()
{

}

void LevelManager::Render()
{
	Draw_NewLevel();
}

void LevelManager::Open_NewLevelWindow()
{
	m_DrawWindow_NewLevel = true;
}

void LevelManager::Draw_NewLevel()
{
	if (m_DrawWindow_NewLevel == true)
	{
		if (ImGui::Begin("Create New Level"))
		{
			ImGui::InputText("Level Name", m_NewLevelName.data(), 32);

			if (ImGui::Button("Create Level"))
			{
				FLOW_ENGINE_LOG("TODO: Create new level.");
			}
		}
		ImGui::End();
	}

}
