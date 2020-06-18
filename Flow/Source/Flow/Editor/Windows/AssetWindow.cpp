#include "Flowpch.h"
#include "AssetWindow.h"
#include "ThirdParty/ImGui/imgui.h"
#include "Flow/Assets/AssetSystem.h"

AssetWindow::AssetWindow()
{
	_SelectedDirectory = AssetSystem::GetAssetDirectory().append("Assets");
}

void AssetWindow::DrawWindow()
{
	if (ImGui::Begin("Asset Browser"))
	{
		float TotalWidth = ImGui::GetContentRegionAvail().x;
		float FolderWidth = (TotalWidth - 15) / 3;
		ImGuiWindowFlags WindowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_AlwaysVerticalScrollbar | ImGuiWindowFlags_AlwaysHorizontalScrollbar;

		if (ImGui::BeginChild(ImGui::GetID("AssetBrowser - Folders"),ImVec2(FolderWidth, 0), true, WindowFlags))
		{
			if (ImGui::BeginMenuBar())
			{
				ImGui::TextUnformatted("Folder Structure");
				ImGui::EndMenuBar();
			}

			if (ImGui::TreeNode("Editor Assets"))
			{
				_CurrentParentDirectory = AssetSystem::GetAssetDirectory(true).append("Assets");
				DrawDirectory(_CurrentParentDirectory);
				ImGui::TreePop();
			}

			if (ImGui::TreeNode("Application Assets"))
			{
				_CurrentParentDirectory = AssetSystem::GetAssetDirectory(true).append("Assets");
				DrawDirectory(_CurrentParentDirectory);
				ImGui::TreePop();
			}
		}
		ImGui::EndChild();

		ImGui::SameLine();

		if (ImGui::BeginChild(ImGui::GetID("AssetBrowser - Files"), ImVec2(FolderWidth * 2, 0), true, WindowFlags))
		{
			if (ImGui::BeginMenuBar())
			{
				ImGui::TextUnformatted("File Browser");
				ImGui::EndMenuBar();
			}

			//std::string String = AssetSystem::GetRootDirectory().string();
			//ImGui::Text(String.c_str());

			for (auto& Element : std::filesystem::directory_iterator(_SelectedDirectory, std::filesystem::directory_options::skip_permission_denied))
			{
				ImGui::Button(Element.path().filename().string().c_str());
			}
		}
		ImGui::EndChild();
	}
	ImGui::End();
}

void AssetWindow::DrawDirectory(const std::filesystem::path& CurrentPath)
{
	for (auto& Element : std::filesystem::directory_iterator(CurrentPath, std::filesystem::directory_options::skip_permission_denied))
	{
		bool IsDirectory = Element.is_directory();
		//If this is a directory, draw as part of tree, otherwise draw as label.
		if (IsDirectory)
		{
			auto RelativePath = std::filesystem::relative(Element.path(), _CurrentParentDirectory);
			if (ImGui::TreeNode(RelativePath.string().c_str()))
			{
				if (ImGui::IsItemClicked())
					_SelectedDirectory = Element.path();

				//If a directory, draw the children before proceeding
				DrawDirectory(Element.path());
				ImGui::TreePop();
			}
		}
	}
}