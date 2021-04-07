#include "pch.h"
#include "ShaderAsset.h"
#include "Framework/Utils/FileSystem.h"

ShaderAsset::ShaderAsset()
{
}

bool ShaderAsset::ImportAsset(const std::string& FilePath, const std::string& SavePath)
{
	m_ShaderPath = FilePath;
	m_AssetSize = sizeof(m_ShaderPath);

	fs::path ConvertedPath = SavePath;
	ConvertedPath.replace_extension(".cso");

	SaveAsset(ConvertedPath.string());

	return true;
}

bool ShaderAsset::LoadAsset(const std::string& AssetPath)
{
	m_ShaderPath = AssetPath;
	m_AssetSize = sizeof(m_ShaderPath);

	return true;
}

const std::string& ShaderAsset::GetPath() const
{
	return m_ShaderPath;
}
