#pragma once

// Includes /////////////////////////////////////////////////////////////////////

#include <string>
#include "Flow\Assets\Asset.h"

// Class Definition /////////////////////////////////////////////////////////////

class FLOW_API ShaderAsset : public Asset
{
public:

	// Public Functions /////////////////////////////////////////////////////////

								ShaderAsset();

	virtual bool				ImportAsset(const std::string& FilePath, const std::string& SavePath) override;
	virtual bool				LoadAsset(const std::string& AssetPath) override;
	const std::string&			GetPath() const;

private:

	// Private Functions /////////////////////////////////////////////////////////

	std::string					m_shaderPath;
};