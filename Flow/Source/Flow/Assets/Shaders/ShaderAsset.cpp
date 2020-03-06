#include "Flowpch.h"
#include "ShaderAsset.h"
#include "Flow\Application.h"

namespace Flow
{
	ShaderAsset::ShaderAsset()
	{
	}

	bool ShaderAsset::LoadAsset(const std::string& AssetPath)
	{
		m_ShaderPath = Application::GetApplication().GetLocalFilePath() + AssetPath;
		m_AssetSize = sizeof(m_ShaderPath);

		return true;
	}

	const std::string& ShaderAsset::GetPath() const
	{
		return m_ShaderPath;
	}

}
