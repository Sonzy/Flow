//= Includes ====================================================

#include "pch.h"

#if WITH_EDITOR

#include "AssetBrowser.h"
#include "Assets/AssetSystem.h"
#include "Rendering/Core/Bindables/Texture.h"
#include "ThirdParty/ImGui/imgui.h"

#include "Application.h"
#include "Window/Window_Win32.h"

#include "Editor/Editor.h"

//= Class Definition - AssetBrowser ==============================

AssetBrowser::AssetBrowser()
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

	m_Icon_Mesh = new Bindables::Texture(AssetSystem::GetAsset<TextureAsset>("Icon_FMesh"), 1);
	m_Icon_Shader = new Bindables::Texture(AssetSystem::GetAsset<TextureAsset>("Icon_Shader"), 1);
	m_Icon_Texture = new Bindables::Texture(AssetSystem::GetAsset<TextureAsset>("Icon_Image"), 1);
	m_Icon_Folder = new Bindables::Texture(AssetSystem::GetAsset<TextureAsset>("Icon_Folder"), 1);
}

AssetBrowser::~AssetBrowser()
{
	delete m_Icon_Mesh;
	delete m_Icon_Shader;
	delete m_Icon_Texture;
	delete m_Icon_Folder;
}

void AssetBrowser::Update()
{

}

void AssetBrowser::Render()
{
	PROFILE_FUNCTION();

	static float previousFrameWindowHeight = 0.0f;

	//TODO: Make centralised
	Editor::Settings& editorSettings = Editor::GetSettings();
	if (editorSettings.m_DockFloatingWindows == true)
	{
		//Bottom Left
		Vector2 position = Editor::Get().GetSceneWindowPosition();
		position.y += Editor::Get().GetSceneWindowSize().y;
		ImGui::SetNextWindowPos(ImVec2(position.x + editorSettings.m_DockPadding.x, position.y - editorSettings.m_DockPadding.y - previousFrameWindowHeight));
	}

	if (ImGui::Begin("Asset Browser Editor", nullptr, ImGuiWindowFlags_NoScrollbar))
	{
		if (ImGui::BeginTabBar("DirectoryWindow"))
		{
			if (ImGui::BeginTabItem("Editor"))
			{
				DrawDirectoryBar(m_SelectedEditorDirectory, true);

				if (ImGui::BeginChild("EditorBrowser", ImVec2(0, 0), true, ImGuiWindowFlags_AlwaysVerticalScrollbar))
				{
					m_EditorMode = true;
					DrawDirectory(m_SelectedEditorDirectory);

					DrawContextWindow();
				}
				ImGui::EndChild();

				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("Game"))
			{
				DrawDirectoryBar(m_SelectedDirectory, false);

				if (ImGui::BeginChild("Browser", ImVec2(0, 0), true, ImGuiWindowFlags_AlwaysVerticalScrollbar))
				{
					m_EditorMode = false;
					DrawDirectory(m_SelectedDirectory);

					DrawContextWindow();
				}
				ImGui::EndChild();

				ImGui::EndTabItem();
			}

			ImGui::EndTabBar();
		}

		previousFrameWindowHeight = ImGui::GetWindowSize().y;
	}
	ImGui::End();
}

void AssetBrowser::DrawDirectory(const FilePath& CurrentPath)
{
	std::vector<std::pair<FilePath, FilePath>> Renames;
	m_WindowWidth = ImGui::GetContentRegionAvailWidth();
	m_CursorInitialX = ImGui::GetCursorPosX();
	int Count = 0;

	//Draw folders first
	std::vector<fs::path> Folders;
	std::vector<fs::path> Files;

	for (auto& Element : fs::directory_iterator(CurrentPath, fs::directory_options::skip_permission_denied))
	{
		if (Element.is_directory() == true)
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

void AssetBrowser::DrawFolder(const FilePath& CurrentPath, int UniqueID, std::vector<std::pair<FilePath, FilePath>>& OutRenameList)
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
	int LineCount = static_cast<int>(ceil(ItemSize.x / m_IconSize));
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

void AssetBrowser::DrawEntry(const FilePath& CurrentPath, int UniqueID, std::vector<std::pair<FilePath, FilePath>>& OutRenameList)
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
	if (const Bindables::Texture* tex = GetTextureForExtension(CurrentPath.filename()))
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
			FLOW_ENGINE_ERROR("Unable to parse file type %s", ExtensionString.c_str());
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
	int LineCount = static_cast<int>(ceil(ItemSize.x / m_IconSize));
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

void AssetBrowser::DrawContextWindow()
{
	if (ImGui::BeginPopupContextWindow("AssetBrowserContext"))
	{
		if (ImGui::MenuItem("Import Asset"))
		{
			std::string FilePathString;
			if (static_cast<Window_Win32&>(Application::Get().GetWindow()).OpenFile(FilePathString) == true)
			{
				FilePath path = FilePathString;
				std::string FileName = path.filename().string();
				AssetSystem::ImportAsset(FilePathString, m_EditorMode ? m_SelectedEditorDirectory.string() + '\\' + FileName : m_SelectedDirectory.string() + '\\' + FileName); //TODO: Meshes being imported to asset map as .obj not .fmesh
			}
			else
			{
				FLOW_ENGINE_ERROR("AssetBrowser::DrawWindow: Failed to import file");
			}

		}
		ImGui::EndPopup();
	}
}

const Bindables::Texture* AssetBrowser::GetTextureForExtension(const FilePath& FileName) const
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
			if (const Bindables::Texture* Thumb = Tex->GetThumbnail())
			{
				return Thumb;
			}
		}
		return m_Icon_Texture;
	}

	return nullptr;
}

void AssetBrowser::AddToRenameQueue(const FilePath& ParentDirectory, const std::string& OldFilename, const std::string& NewFilename, std::vector<std::pair<FilePath, FilePath>>& OutRenameList)
{
	OutRenameList.emplace_back(ParentDirectory.string() + "\\" + OldFilename, ParentDirectory.string() + "\\" + NewFilename);
}

void AssetBrowser::DrawDirectoryBar(const FilePath Parent, bool Editor)
{
	//Get the start location relative to the parent folder
	std::string StringDirectory = Parent.string();
	std::string ParentStringDirectory = Editor ? AssetSystem::GetEngineAssetDirectory().string() : AssetSystem::GetGameAssetDirectory().string();
	size_t Position = StringDirectory.find(ParentStringDirectory);

	fs::path Start = Parent;
	if (Position != -1)
	{																						//Moving back to allow /Assets/ TODO: Dont hardcode this?
		Start = fs::path(StringDirectory.begin() + Position + ParentStringDirectory.length() - 6, StringDirectory.end());
	}

	//Start the iterator from the Asset Directory
	fs::path::iterator it(Start.begin());
	fs::path::iterator it_next(Start.begin()); it_next++;
	fs::path::iterator it_end(Start.end());
	fs::path current;

	//If we have no subdirectories, skip a line
	if (it == it_end)
	{
		ImGui::NewLine();
	}

	//Draw the Path
	for (; it != it_end; it++)
	{
		std::string pathString = it->string();

		//Skip any caught / paths
		if (strcmp("/", pathString.c_str()) == 0 || strcmp("\\", pathString.c_str()) == 0)
		{
			it_next++;
			continue;
		}

		current /= *it;

		if (ImGui::Button(pathString.c_str()))
		{
			if (Editor)
			{
				if (m_SelectedEditorDirectory != AssetSystem::GetEngineAssetDirectory())
				{
					m_SelectedEditorDirectory = AssetSystem::GetEngineAssetParentDirectory() / current;
					break;
				}
			}
			else
			{
				if (m_SelectedDirectory != AssetSystem::GetGameAssetDirectory())
				{
					m_SelectedDirectory = AssetSystem::GetGameAssetParentDirectory() / current;
					break;
				}
			}

		}

		if (it_next != it_end)
		{
			ImGui::SameLine();
			ImGui::Text("/");
			ImGui::SameLine();
			it_next++;
		}
	}
}

#endif // WITH_EDITOR