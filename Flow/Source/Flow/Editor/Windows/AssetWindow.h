#pragma once
#include <filesystem>

class AssetWindow
{
public:

	AssetWindow();

	void DrawWindow();

private:
	void DrawDirectory(const std::filesystem::path& CurrentPath);

	std::filesystem::path _SelectedDirectory;
	std::filesystem::path _CurrentParentDirectory;
};