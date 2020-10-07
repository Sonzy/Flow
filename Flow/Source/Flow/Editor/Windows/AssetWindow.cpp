//= Includes ====================================================

#include "Flowpch.h"
#include "AssetWindow.h"
#include "Assets/AssetSystem.h"
#include "Rendering/Core/Bindables/Texture.h"
#include "ThirdParty/ImGui/imgui.h"

#include "Application.h"
#include "Window/Window_Win32.h"

//= Class Definition - AssetWindow ==============================

AssetWindow::AssetWindow()
	: m_IconSize(64.0f)
	, m_IconSpacing(20.0f)
	, m_YSpacing(50.0f)
	, m_EditorMode(false)
{
	m_SelectedDirectory = AssetSystem::GetGameAssetDirectory();
	m_SelectedEditorDirectory = AssetSystem::GetEngineAssetDirectory();

	m_Icon_Mesh = new Texture(AssetSystem::GetAsset<TextureAsset>("Icon_FMesh"), 1);
	m_Icon_Shader = new Texture(AssetSystem::GetAsset<TextureAsset>("Icon_Shaders"), 1);
	m_Icon_Texture = new Texture(AssetSystem::GetAsset<TextureAsset>("Icon_Image"), 1);
	m_Icon_Folder = new Texture(AssetSystem::GetAsset<TextureAsset>("Icon_Folder"), 1);
}

void AssetWindow::DrawWindow()
{
	PROFILE_FUNCTION();

	if (ImGui::Begin("Asset Browser"))
	{
		if (ImGui::BeginTabBar("DirectoryWindow"))
		{
			if (ImGui::BeginTabItem("Editor"))
			{
				if (ImGui::Button("Back"))
				{
					//TODO: Safety check so you cant go out of the project
					if (m_SelectedEditorDirectory != AssetSystem::GetEngineAssetDirectory())
					{
						m_SelectedEditorDirectory = m_SelectedEditorDirectory.parent_path();
					}
				}

				m_EditorMode = true;
				DrawDirectory(m_SelectedEditorDirectory);
				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("Game"))
			{
				if (ImGui::Button("Back"))
				{
					//TODO: Safety check so you cant go out of the project
					if (m_SelectedDirectory != AssetSystem::GetGameAssetDirectory())
					{
						m_SelectedDirectory = m_SelectedDirectory.parent_path();
					}
				}

				m_EditorMode = false;
				DrawDirectory(m_SelectedDirectory);
				ImGui::EndTabItem();
			}

			ImGui::EndTabBar();
		}

		if (ImGui::BeginPopupContextWindow("AssetWindowContext"))
		{
			if(ImGui::MenuItem("Import Asset"))
			{
				std::string FilePathString;
				if (static_cast<Window_Win32&>(Application::Get().GetWindow()).OpenFile(FilePathString) == true)
				{
					FilePath path = FilePathString;		
					std::string FileName = path.filename().string();
					AssetSystem::ImportAsset(FilePathString, m_EditorMode ? m_SelectedEditorDirectory.string() + FileName : m_SelectedDirectory.string() + FileName);
				}
				else
				{
					FLOW_ENGINE_ERROR("AssetWindow::DrawWindow: Failed to import file");
				}

			}

			ImGui::EndPopup();
		}
	}
	ImGui::End();
}

void AssetWindow::DrawDirectory(const FilePath& CurrentPath)
{
	std::vector<std::pair<FilePath, FilePath>> Renames;
	float WindowWidth = ImGui::GetContentRegionAvailWidth();
	float CursorInitialX = ImGui::GetCursorPosX();
	char buffer[128];
	int Count = 0;

	//Draw folders first
	std::vector<fs::path> Folders;
	std::vector<fs::path> Files;

	for (auto& Element : fs::directory_iterator(CurrentPath, fs::directory_options::skip_permission_denied))
	{
		if(Element.is_directory() == true)
		{
			Folders.push_back(Element.path());
		}
		else
		{
			Files.push_back(Element.path());
		}
	}

	for (fs::path Folder : Folders)
	{
		float CursorStartX = ImGui::GetCursorPosX();
		float CursorStartY = ImGui::GetCursorPosX();

		ImGui::PushID(Count);
		if (ImGui::ImageButton(m_Icon_Folder->GetTextureView(), ImVec2(m_IconSize, m_IconSize)))
		{
			if (m_EditorMode)
			{
				m_SelectedEditorDirectory = Folder;
			}
			else
			{
				m_SelectedDirectory = Folder;
			}
		}
		ImGui::PopID();

		ImGui::SameLine();


		//Move Cursor
		ImVec2 CursorPos = ImGui::GetCursorPos();
		ImGui::SetCursorPosX(CursorStartX);
		ImGui::SetCursorPosY(CursorPos.y + 100.0f);

		// Draw name
		FilePath FolderName = Folder.stem();
		std::string FilenameString = FolderName.string();
		ImGui::PushItemWidth(m_IconSize);

		sprintf_s(buffer, "###Label%d", Count);
		char namebuffer[128];
		sprintf_s(namebuffer, "%s", FilenameString.c_str());

		if (ImGui::InputText(buffer, namebuffer, 128, ImGuiInputTextFlags_EnterReturnsTrue))
		{
			if (strlen(namebuffer) > 0)
			{
				//TODO: Make sure it is a valid name
				std::string originalString = CurrentPath.string() + "\\" + FilenameString;
				std::string pathString = CurrentPath.string() + "\\" + namebuffer;
				Renames.emplace_back(std::move(originalString), std::move(pathString));
			}
		}

		ImGui::SetCursorPos(CursorPos);

		if (ImGui::GetCursorPosX() + m_IconSpacing + m_IconSize > WindowWidth)
		{
			ImGui::SetCursorPos(ImVec2(CursorInitialX, CursorPos.y + m_IconSize + m_YSpacing));
		}

		Count++;
	}

	for (fs::path File : Files)
	{
		float CursorStartX = ImGui::GetCursorPosX();
		float CursorStartY = ImGui::GetCursorPosX();

		//Skip unloaded assets
		std::string SearchString = File.stem().string();
		if (AssetSystem::DoesAssetExist(SearchString) == false)
		{
			continue;
		}

		std::string ExtensionString = File.extension().string();

		//=Draw File Image

		Texture* tex = nullptr;
		if (AssetSystem::HasExtension(ExtensionString.c_str(), ".fmesh"))
		{
			tex = m_Icon_Mesh;
		}
		else if (AssetSystem::HasExtension(ExtensionString.c_str(), ".cso"))
		{
			tex = m_Icon_Shader;
		}
		else if (AssetSystem::HasExtension(ExtensionString.c_str(), ".png"))
		{
			tex = m_Icon_Texture;
		}

		if (tex)
		{
			ImGui::PushID(Count);
			if (ImGui::ImageButton(tex->GetTextureView(), ImVec2(m_IconSize, m_IconSize)))
			{
				//TODO:
			}
			ImGui::PopID();
		}
		else
		{
			if (const bool ErrorOnUnknownFile = false)
			{
				FLOW_ENGINE_ERROR("Unable to parse file type {0}", ExtensionString);
			}
			continue;
		}

		ImGui::SameLine();



		// Move Cursor
		ImVec2 CursorPos = ImGui::GetCursorPos();
		ImGui::SetCursorPosX(CursorStartX);
		ImGui::SetCursorPosY(CursorPos.y + 100.0f);

		// Draw File name
		FilePath Filename = File.stem();
		std::string FilenameString = Filename.string();

		ImGui::PushItemWidth(m_IconSize);

		sprintf_s(buffer, "###Label%d", Count);
		char namebuffer[128];
		sprintf_s(namebuffer, "%s", FilenameString.c_str());

		if (ImGui::InputText(buffer, namebuffer, 128, ImGuiInputTextFlags_EnterReturnsTrue))
		{
			if (strlen(namebuffer) > 0)
			{
				//TODO: Make sure it is a valid name
				std::string originalString = CurrentPath.string() + "\\" + FilenameString + ExtensionString;
				std::string pathString = CurrentPath.string() + "\\" + namebuffer + ExtensionString;
				Renames.emplace_back(std::move(originalString), std::move(pathString));
			}
		}

		ImGui::SetCursorPos(CursorPos);

		if (ImGui::GetCursorPosX() + m_IconSpacing + m_IconSize > WindowWidth)
		{
			ImGui::SetCursorPos(ImVec2(CursorInitialX, CursorPos.y + m_IconSize + m_YSpacing));
		}

		Count++;
	}

	for (auto& RenameData : Renames)
	{
		fs::rename(RenameData.first, RenameData.second);
		AssetSystem::UpdateAssetName(RenameData.first.string(), RenameData.second.string());
	}
}