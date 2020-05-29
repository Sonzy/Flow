#include "Flowpch.h"
#include "ShaderAsset.h"
#include "Flow\Application.h"

ShaderAsset::ShaderAsset()
{
}

bool ShaderAsset::LoadAsset(const std::string& AssetPath)
{
	_ShaderPath = Application::GetApplication().GetLocalFilePath() + AssetPath;
	_AssetSize = sizeof(_ShaderPath);

	return true;
}

const std::string& ShaderAsset::GetPath() const
{
	return _ShaderPath;
}
