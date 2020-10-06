#pragma once
#include "Flow\Assets\Asset.h"

/* For now, shader asset just stores the path to the shader since I need a platform agnostic way of storing the shader,
not sure how other platforms handle shaders so will set up later */
class FLOW_API ShaderAsset : public Asset
{
public:

	//= Public Functions =========================

								ShaderAsset();

	virtual bool				ImportAsset(const std::string& AssetPath) override;
	virtual bool				LoadAsset(const std::string& AssetPath) override;
	const std::string&			GetPath() const;

private:

	std::string					m_ShaderPath;
};