#pragma once

// Includes ///////////////////////////////////////////


#include "UIComponent.h"
#include "Framework/Utils/FileSystem.h"

// Forward Declarations ///////////////////////////////

class Texture;

// Class Definition ///////////////////////////////////

class AssetBrowser : public UIComponent
{
public:

	// Public Functions ///////////////////////////////////

							AssetBrowser();
	virtual					~AssetBrowser();

	//= UI Component Interface =

	virtual void			Update() override;
	virtual void			Render() override;

private:

	// Private Functions ///////////////////////////////////

	void					DrawDirectory(const FilePath& CurrentPath);
	void					DrawFolder(const FilePath& CurrentPath, int UniqueID, std::vector<std::pair<FilePath, FilePath>>& OutRenameList);
	void					DrawEntry(const FilePath& CurrentPath, int UniqueID, std::vector<std::pair<FilePath, FilePath>>& OutRenameList);
	void					DrawContextWindow();

	const Texture*			GetTextureForExtension(const FilePath& FileName) const;

	//TODO: Better renaming
	void					AddToRenameQueue(const FilePath& ParentDirectory, const std::string& OldFilename, const std::string& NewFilename, std::vector<std::pair<FilePath, FilePath>>& OutRenameList);

	void					DrawDirectoryBar(const FilePath Parent, bool Editor);

private:

	// Private Variables ///////////////////////////////////

	bool					m_EditorMode;
	FilePath				m_SelectedEditorDirectory;
	FilePath				m_SelectedDirectory;
	FilePath				m_CurrentParentDirectory;

	Texture*				m_Icon_Mesh;
	Texture*				m_Icon_Shader;
	Texture*				m_Icon_Texture;
	Texture*				m_Icon_Folder;

	//= Formatting =

	float					m_IconSize;
	float					m_IconSpacing;
	float					m_YSpacing;
	float					m_TextYOffset; //Y Offset from the icon 
	float					m_NameBoxHeight; //Y Offset from the icon 
	int						m_MaxNameLines; //Y Offset from the icon 

	//= Imgui Helper stuff =

	char					m_UIDBuffer[128];
	float					m_CursorInitialX;
	float					m_WindowWidth;
};