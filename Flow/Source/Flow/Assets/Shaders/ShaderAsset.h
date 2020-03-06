#pragma once
#include "Flow\Assets\AssetBase.h"

namespace Flow
{
	/* For now, shader asset just stores the path to the shader since I need a platform agnostic way of storing the shader, not sure how other platforms handle shaders so will set up later */
	class ShaderAsset : public AssetBase
	{
	public:
		ShaderAsset();

		virtual bool LoadAsset(const std::string& AssetPath) override;

		const std::string& GetPath() const;

	private:

		std::string m_ShaderPath;
	};
}