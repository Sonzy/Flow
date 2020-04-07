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
		ShaderPath_ = Application::GetApplication().GetLocalFilePath() + AssetPath;
		AssetSize_ = sizeof(ShaderPath_);

		return true;
	}

	const std::string& ShaderAsset::GetPath() const
	{
		return ShaderPath_;
	}

}
