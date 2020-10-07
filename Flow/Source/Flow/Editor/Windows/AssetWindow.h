#pragma once

//= Includes ==============================================

#include "Utils/FileSystem.h"

//= Forward Declaration ==============================================

class Texture;

//= Class Definition ==============================================

class AssetWindow
{
public:

	//= Public Functions ==================================

										AssetWindow();
	void								DrawWindow();

private:

	//= Private Functions =================================

	void DrawDirectory(const FilePath& CurrentPath);

private:
	//= Private Variables =================================

	bool								m_EditorMode;
	FilePath							m_SelectedEditorDirectory;
	FilePath							m_SelectedDirectory;
	FilePath							m_CurrentParentDirectory;
	
	Texture*							m_Icon_Mesh;
	Texture*							m_Icon_Shader;
	Texture*							m_Icon_Texture;
	Texture*							m_Icon_Folder;

	//= Formatting =

	float								m_IconSize;
	float								m_IconSpacing;
	float								m_YSpacing;
};