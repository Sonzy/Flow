#include "Flowpch.h"
#include "LevelManager.h"
#include "ThirdParty/ImGui/imgui.h"

LevelManager::LevelManager()
	: _Draw_NewLevelWindow(false)
{
	_NewLevelName.resize(32);
}

void LevelManager::Open_NewLevelWindow()
{
	_Draw_NewLevelWindow = true;
}

void LevelManager::DrawWindows()
{
	if(_Draw_NewLevelWindow) Draw_NewLevelWindow();
}

void LevelManager::Draw_NewLevelWindow()
{
	if (ImGui::Begin("Create New Level"))
	{
		ImGui::InputText("Level Name", _NewLevelName.data(), 32);
		if (ImGui::Button("Create Level"))
			FLOW_ENGINE_LOG("TODO: Create new level.");
	}
	ImGui::End();
}
