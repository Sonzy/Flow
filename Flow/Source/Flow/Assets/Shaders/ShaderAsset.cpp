#include "Flowpch.h"
#include "ShaderAsset.h"
#include "Flow\Application.h"
#include "Flow/Assets/AssetSystem.h"

ShaderAsset::ShaderAsset()
{
}

bool ShaderAsset::LoadAsset(const std::string& AssetPath)
{
	_ShaderPath = AssetPath;
	_AssetSize = sizeof(_ShaderPath);

	return true;
}

const std::string& ShaderAsset::GetPath() const
{
	return _ShaderPath;
}
