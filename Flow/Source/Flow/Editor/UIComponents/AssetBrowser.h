#pragma once

#if WITH_EDITOR

// Includes ///////////////////////////////////////////

#include "UIComponent.h"
#include "Framework/Utils/FileSystem.h"

// Forward Declarations ///////////////////////////////

class Asset;

namespace Bindables
{
	class Texture;
}

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

	const Bindables::Texture*	GetTextureForExtension(const FilePath& FileName) const;

	//TODO: Better renaming
	void					AddToRenameQueue(const FilePath& ParentDirectory, const std::string& OldFilename, const std::string& NewFilename, std::vector<std::pair<FilePath, FilePath>>& OutRenameList);

	void					DrawDirectoryBar(const FilePath Parent, bool Editor);

	void					DrawPropertiesWindow();

private:

	// Private Variables ///////////////////////////////////

	bool					m_editorMode;
	FilePath				m_selectedEditorDirectory;
	FilePath				m_selectedDirectory;
	FilePath				m_currentParentDirectory;

	Bindables::Texture*		m_icon_Mesh;
	Bindables::Texture*		m_icon_Shader;
	Bindables::Texture*		m_icon_Texture;
	Bindables::Texture*		m_icon_Folder;

	//= Formatting =

	float					m_iconSize;
	float					m_iconSpacing;
	float					m_ySpacing;
	float					m_textYOffset; //Y Offset from the icon 
	float					m_nameBoxHeight; //Y Offset from the icon 
	int						m_maxNameLines; //Y Offset from the icon 

	//= Imgui Helper stuff =

	char					m_uIDBuffer[128];
	float					m_cursorInitialX;
	float					m_windowWidth;

	//= Properties Window =

	const Asset*			m_propertiesAsset;
};

#endif WITH_EDITOR