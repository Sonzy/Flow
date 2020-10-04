//= Includes ====================================================

#include "Flowpch.h"
#include "AssetWindow.h"
#include "Assets/AssetSystem.h"
#include "ThirdParty/ImGui/imgui.h"

//= Class Definition - AssetWindow ==============================

AssetWindow::AssetWindow()
{
	m_SelectedDirectory = AssetSystem::GetAssetDirectory().append("Assets");
}

void AssetWindow::DrawWindow()
{
	PROFILE_FUNCTION();

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
				m_CurrentParentDirectory = AssetSystem::GetAssetDirectory(true).append("Assets");
				DrawDirectory(m_CurrentParentDirectory);
				ImGui::TreePop();
			}

			if (ImGui::TreeNode("Application Assets"))
			{
				m_CurrentParentDirectory = AssetSystem::GetAssetDirectory(false).append("Assets");
				DrawDirectory(m_CurrentParentDirectory);
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

			for (const std::filesystem::directory_entry& Element : std::filesystem::directory_iterator(m_SelectedDirectory, std::filesystem::directory_options::skip_permission_denied))
			{
#if 1
				if (Element.path().extension() == ".FMesh")
				{
					ImGui::Button(Element.path().filename().string().c_str());
				}
#else
				ImGui::Button(Element.path().filename().string().c_str());
#endif
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
			auto RelativePath = std::filesystem::relative(Element.path(), m_CurrentParentDirectory);
			bool TreeOpen = ImGui::TreeNode(RelativePath.string().c_str());

			if (ImGui::IsItemClicked())
				m_SelectedDirectory = Element.path();

			if(TreeOpen)
			{
				//If a directory, draw the children before proceeding
				DrawDirectory(Element.path());
				ImGui::TreePop();
			}
		}
	}
}