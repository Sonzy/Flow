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
	: m_IconSize(100.0f)
	, m_IconSpacing(20.0f)
	, m_YSpacing(50.0f)
	, m_EditorMode(false)
	, m_TextYOffset(15.0f)
	, m_NameBoxHeight(50.0f)
	, m_MaxNameLines(3)
{
	m_SelectedDirectory = AssetSystem::GetGameAssetDirectory();
	m_SelectedEditorDirectory = AssetSystem::GetEngineAssetDirectory();

	m_Icon_Mesh = new Texture(AssetSystem::GetAsset<TextureAsset>("Icon_FMesh"), 1);
	m_Icon_Shader = new Texture(AssetSystem::GetAsset<TextureAsset>("Icon_Shader"), 1);
	m_Icon_Texture = new Texture(AssetSystem::GetAsset<TextureAsset>("Icon_Image"), 1);
	m_Icon_Folder = new Texture(AssetSystem::GetAsset<TextureAsset>("Icon_Folder"), 1);
}

void AssetWindow::DrawWindow()
{
	PROFILE_FUNCTION();

	if (ImGui::Begin("Asset Browser Editor", nullptr, ImGuiWindowFlags_NoScrollbar))
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

			static bool open = false;
			if (ImGui::BeginTabItem("Options"))
			{		
				ImGui::Checkbox("Popout", &open);

				ImGui::InputFloat("Icon Size", &m_IconSize);
				ImGui::InputFloat("Icon Spacing", &m_IconSpacing);
				ImGui::InputFloat("Y Spacing", &m_YSpacing);
				ImGui::InputFloat("Text Y Offset", &m_TextYOffset);
				ImGui::InputFloat("Name Textbox Height", &m_NameBoxHeight);
				ImGui::InputInt("Max Name Lines", &m_MaxNameLines);

				ImGui::EndTabItem();
			}
			ImGui::EndTabBar();

			if (open && ImGui::Begin("Popout"))
			{
				ImGui::InputFloat("Icon Size", &m_IconSize);
				ImGui::InputFloat("Icon Spacing", &m_IconSpacing);
				ImGui::InputFloat("Y Spacing", &m_YSpacing);
				ImGui::InputFloat("Text Y Offset", &m_TextYOffset);
				ImGui::InputFloat("Name Textbox Height", &m_NameBoxHeight);
				ImGui::InputInt("Max Name Lines", &m_MaxNameLines);

				ImGui::End();
			}
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
					AssetSystem::ImportAsset(FilePathString, m_EditorMode ? m_SelectedEditorDirectory.string() + '\\' + FileName : m_SelectedDirectory.string() + '\\' + FileName);
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
	m_WindowWidth = ImGui::GetContentRegionAvailWidth();
	m_CursorInitialX = ImGui::GetCursorPosX();
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
		DrawFolder(Folder, Count, Renames);

		Count++;
	}

	for (fs::path File : Files)
	{
		DrawEntry(File, Count, Renames);

		Count++;
	}

	for (auto& RenameData : Renames)
	{
		fs::rename(RenameData.first, RenameData.second);
		AssetSystem::UpdateAssetName(RenameData.first.string(), RenameData.second.string());
	}
}

void AssetWindow::DrawFolder(const FilePath& CurrentPath, int UniqueID, std::vector<std::pair<FilePath, FilePath>>& OutRenameList)
{
	float CursorStartX = ImGui::GetCursorPosX();
	float CursorStartY = ImGui::GetCursorPosX();

	ImGui::PushID(UniqueID);
	if (ImGui::ImageButton(m_Icon_Folder->GetTextureView(), ImVec2(m_IconSize, m_IconSize), ImVec2(0, 0), ImVec2(1, 1)))
	{
		if (m_EditorMode)
		{
			m_SelectedEditorDirectory = CurrentPath;
		}
		else
		{
			m_SelectedDirectory = CurrentPath;
		}
	}
	ImGui::PopID();

	ImGui::SameLine();


	//Move Cursor
	ImVec2 CursorPos = ImGui::GetCursorPos();
	ImGui::SetCursorPosX(CursorStartX);
	ImGui::SetCursorPosY(CursorPos.y + m_IconSize + m_TextYOffset);

	// Draw name
	FilePath FolderName = CurrentPath.stem();
	std::string FilenameString = FolderName.string();

	//Draw the centred filename text - TODO: Cleanup
	ImVec2 ItemSize = ImGui::CalcTextSize(FilenameString.c_str());
	int LineCount = ceil(ItemSize.x / m_IconSize);
	float centreOffset = (ItemSize.x > m_IconSize) ? ImGui::GetStyle().FramePadding.x : ((m_IconSize - ItemSize.x) / 2.0f) + ImGui::GetStyle().FramePadding.x;
	ImGui::SetCursorPosX(ImGui::GetCursorPosX() + centreOffset);
	ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + m_IconSize);
	ImGui::PushClipRect(ImGui::GetCursorScreenPos(), ImVec2(ImGui::GetCursorScreenPos().x + m_IconSize, ImGui::GetCursorScreenPos().y + (m_MaxNameLines * ImGui::GetTextLineHeight())), true);
	ImGui::Text("%s", FilenameString.c_str());
	ImGui::PopClipRect();
	ImGui::PopTextWrapPos();

	//Reset the cursor

	ImGui::SetCursorPos(ImVec2(CursorPos.x + m_IconSpacing, CursorPos.y));

	if (ImGui::GetCursorPosX() + m_IconSpacing + m_IconSize > m_WindowWidth)
	{
		ImGui::SetCursorPos(ImVec2(m_CursorInitialX, CursorPos.y + m_IconSize + m_YSpacing));
	}
}

void AssetWindow::DrawEntry(const FilePath& CurrentPath, int UniqueID, std::vector<std::pair<FilePath, FilePath>>& OutRenameList)
{
	float CursorStartX = ImGui::GetCursorPosX();
	float CursorStartY = ImGui::GetCursorPosX();

	//Skip unloaded assets
	std::string SearchString = CurrentPath.stem().string();
	if (AssetSystem::DoesAssetExist(SearchString) == false)
	{
		return;
	}

	//=Draw File Image

	std::string ExtensionString = CurrentPath.extension().string();
	if (const Texture* tex = GetTextureForExtension(CurrentPath.filename()))
	{
		ImGui::PushID(UniqueID);
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

		return;
	}

	ImGui::SameLine();



	// Move Cursor
	ImVec2 CursorPos = ImGui::GetCursorPos();
	ImGui::SetCursorPosX(CursorStartX);
	ImGui::SetCursorPosY(CursorPos.y + m_IconSize + m_TextYOffset);

	// Draw File name
	FilePath Filename = CurrentPath.stem();
	std::string FilenameString = Filename.string();

	ImGui::PushItemWidth(m_IconSize);

	sprintf_s(m_UIDBuffer, "###Label%d", UniqueID);

	//Draw the centred filename text - TODO: Cleanup
	ImVec2 ItemSize = ImGui::CalcTextSize(FilenameString.c_str());
	int LineCount = ceil(ItemSize.x / m_IconSize);
	float centreOffset = (ItemSize.x > m_IconSize) ? ImGui::GetStyle().FramePadding.x : ((m_IconSize - ItemSize.x) / 2.0f) + ImGui::GetStyle().FramePadding.x;
	ImGui::SetCursorPosX(ImGui::GetCursorPosX() + centreOffset);
	ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + m_IconSize);
	ImGui::PushClipRect(ImGui::GetCursorScreenPos(), ImVec2(ImGui::GetCursorScreenPos().x + m_IconSize, ImGui::GetCursorScreenPos().y + (m_MaxNameLines * ImGui::GetTextLineHeight())), true);
	ImGui::Text("%s", FilenameString.c_str());
	ImGui::PopClipRect();
	ImGui::PopTextWrapPos();

	ImGui::SetCursorPos(CursorPos);

	if (ImGui::GetCursorPosX() + m_IconSpacing + m_IconSize > m_WindowWidth)
	{
		ImGui::SetCursorPos(ImVec2(m_CursorInitialX, CursorPos.y + m_IconSize + m_YSpacing));
	}
}

const Texture* AssetWindow::GetTextureForExtension(const FilePath& FileName) const
{
	std::string ExtensionString = FileName.extension().string();
	if (AssetSystem::HasExtension(ExtensionString.c_str(), ".fmesh"))
	{
		return m_Icon_Mesh;
	}

	if (AssetSystem::HasExtension(ExtensionString.c_str(), ".cso"))
	{
		return m_Icon_Shader;
	}

	if (AssetSystem::HasExtension(ExtensionString.c_str(), ".png"))
	{
		if (TextureAsset* Tex = AssetSystem::GetAsset<TextureAsset>(FileName.stem().string()))
		{
			if (const Texture* Thumb = Tex->GetThumbnail())
			{
				return Thumb;
			}
		}
		return m_Icon_Texture;
	}

	return nullptr;
}

void AssetWindow::AddToRenameQueue(const FilePath& ParentDirectory, const std::string& OldFilename, const std::string& NewFilename, std::vector<std::pair<FilePath, FilePath>>& OutRenameList)
{
	OutRenameList.emplace_back(ParentDirectory.string() + "\\" + OldFilename, ParentDirectory.string() + "\\" + NewFilename);
}