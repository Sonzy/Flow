#pragma once

//= Includes ==================================

#include "Assets/Asset.h"
#include "GameFramework/Actor.h"

//= Forward Declarations ======================

//= Class Definition ==========================

class LevelAsset : public Asset
{
public:

	//= Public Virtual Functions =========================================

	virtual							~LevelAsset();
	virtual bool					ImportAsset(const std::string& FilePath, const std::string& SavePath) override;
	virtual bool					SaveAsset(const std::string& AssetPath) override;
	virtual bool					LoadAsset(const std::string& AssetPath) override;

	//= Public Functions =================================================

									LevelAsset();
};