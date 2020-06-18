#include "Flowpch.h"
#include "AssetWindow.h"
#include "ThirdParty/ImGui/imgui.h"
#include "Flow/Assets/AssetSystem.h"

AssetWindow::AssetWindow()
{
}

void AssetWindow::DrawWindow()
{
	if (ImGui::Begin("Asset Browser"))
	{
		float TotalWidth = ImGui::GetContentRegionAvail().x;
		float FolderWidth = (TotalWidth / 4) - 10;
		ImGuiWindowFlags WindowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_AlwaysVerticalScrollbar;

		if (ImGui::BeginChild(ImGui::GetID("AssetBrowser - Folders"),ImVec2(FolderWidth, 0), true, WindowFlags))
		{
			if (ImGui::BeginMenuBar())
			{
				ImGui::TextUnformatted("Folder Structure");
				ImGui::EndMenuBar();
			}

			if (ImGui::TreeNode("Content"))
			{
				if(ImGui::TreeNode("More Content?"))
					ImGui::TreePop();

				ImGui::TreePop();
			}


		}
		ImGui::EndChild();

		ImGui::SameLine();

		if (ImGui::BeginChild(ImGui::GetID("AssetBrowser - Files"), ImVec2(TotalWidth - (FolderWidth + 10), 0), true, WindowFlags))
		{
			if (ImGui::BeginMenuBar())
			{
				ImGui::TextUnformatted("File Browser");
				ImGui::EndMenuBar();
			}

			std::string String = AssetSystem::GetRootDirectory().string();
			ImGui::Text(String.c_str());

			for (int i = 0; i < 100; i++)
			{
				//TODO: Load files
				ImGui::Text("Files will go here");
			}
		}
		ImGui::EndChild();
	}
	ImGui::End();
}
