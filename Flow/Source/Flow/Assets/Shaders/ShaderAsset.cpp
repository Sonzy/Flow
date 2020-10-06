#include "Flowpch.h"
#include "ShaderAsset.h"

ShaderAsset::ShaderAsset()
{
}

bool ShaderAsset::ImportAsset(const std::string& AssetPath)
{
	m_ShaderPath = AssetPath;
	m_AssetSize = sizeof(m_ShaderPath);

	return true;
}

bool ShaderAsset::LoadAsset(const std::string& AssetPath)
{
	return ImportAsset(AssetPath);
}

const std::string& ShaderAsset::GetPath() const
{
	return m_ShaderPath;
}
