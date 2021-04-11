#include "pch.h"
#include "ShaderAsset.h"
#include "Framework/Utils/FileSystem.h"

ShaderAsset::ShaderAsset()
{
}

bool ShaderAsset::ImportAsset(const std::string& FilePath, const std::string& SavePath)
{
	m_shaderPath = FilePath.c_str();
	m_AssetSize = sizeof(m_shaderPath);

	fs::path ConvertedPath = SavePath;
	ConvertedPath.replace_extension(".cso");

	SaveAsset(ConvertedPath.string());

	return true;
}

bool ShaderAsset::LoadAsset(const std::string& AssetPath)
{
	m_shaderPath = AssetPath.c_str();
	m_AssetSize = sizeof(m_shaderPath);

	return true;
}

const string& ShaderAsset::GetPath() const
{
	return m_shaderPath;
}
