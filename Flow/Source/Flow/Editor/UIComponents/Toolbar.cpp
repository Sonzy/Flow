#include "pch.h"
#include "Toolbar.h"

#include "ThirdParty/ImGui/imgui.h"
#include "Flow/Assets/AssetSystem.h"

#include "Flow/Application.h"

ToolBar::ToolBar()
{
	//If we crash here, the texture asset probably doesnt exist
	m_Icon_SelectionTool = new Texture(AssetSystem::GetAsset<TextureAsset>("Icon_Tool_Selection"), 1);
	m_Icon_Play = new Texture(AssetSystem::GetAsset<TextureAsset>("Icon_Play"), 1);
	m_Icon_Pause = new Texture(AssetSystem::GetAsset<TextureAsset>("Icon_Pause"), 1);
	m_Icon_Stop = new Texture(AssetSystem::GetAsset<TextureAsset>("Icon_Stop"), 1);
}

ToolBar::~ToolBar()
{
	delete m_Icon_SelectionTool;
	delete m_Icon_Play;
	delete m_Icon_Pause;
	delete m_Icon_Stop;
}

void ToolBar::Update()
{

}

void ToolBar::Render()
{
	if (ImGui::BeginChild("Toolbar", ImVec2(ImGui::GetContentRegionAvailWidth(), 32), false, ImGuiWindowFlags_NoScrollbar))
	{
		if (m_Icon_SelectionTool != nullptr)
		{
			auto tex = m_Icon_SelectionTool->GetTextureView();
			ImGui::ImageButton(tex, ImVec2(28,28));
		}

		ImGui::SameLine();

		if (m_Icon_Play != nullptr)
		{
			auto tex = m_Icon_Play->GetTextureView();

			if (ImGui::ImageButton(tex, ImVec2(28, 28)))
			{
				Application::SavePlayState();
				Application::StartGame();
			}
		}

		ImGui::SameLine();

		if (m_Icon_Pause != nullptr)
		{
			auto tex = m_Icon_Pause->GetTextureView();
			if (ImGui::ImageButton(tex, ImVec2(28, 28)))
			{
				Application::PauseGame();
			}
		}

		ImGui::SameLine();

		if (m_Icon_Stop != nullptr)
		{
			auto tex = m_Icon_Stop->GetTextureView();
			if (ImGui::ImageButton(tex, ImVec2(28, 28)))
			{
				if (Application::StopGame())
					Application::LoadPlayState();
			}
		}

		ImGui::EndChild();
	}
}
