#pragma once
#include <filesystem>

class AssetWindow
{
public:

	//= Public Functions ==================================

										AssetWindow();
	void								DrawWindow();

private:

	//= Private Functions =================================

	void DrawDirectory(const std::filesystem::path& CurrentPath);

private:
	//= Private Variables =================================

	std::filesystem::path				m_SelectedDirectory;
	std::filesystem::path				m_CurrentParentDirectory;
};