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
		m_ShaderPath = Application::GetApplication().GetLocalFilePathWide() + std::wstring(AssetPath.begin(), AssetPath.end());

		m_AssetSize = sizeof(m_ShaderPath);

		return true;
	}

	const std::wstring& ShaderAsset::GetPath() const
	{
		return m_ShaderPath;
	}

}
