#pragma once
#include <string>

/* Base class for assets. Manages naming and asset types */
class FLOW_API Asset
{
public:

	//= Public Enums ==================================

	enum class Type
	{
		None,
		Mesh,
		Texture,
		Shader
	};

	//= Public Functions ===============================

	virtual bool			LoadAsset(const std::string& LocalPath);

	void					SetAssetName(const std::string& Name)			{ m_AssetName = Name; }
	const std::string&		GetAssetName() const							{ return m_AssetName; }
	Asset::Type				GetAssetType() const							{ return m_AssetType; }
	size_t					GetAssetSize() const							{ return m_AssetSize; };
	// Returns a string with the asset size
	std::string				GetFormattedSize() const;
protected:

	//= Protected Functions ==========================
	std::string				m_AssetName;
	size_t					m_AssetSize = 0;
	Asset::Type				m_AssetType = Asset::Type::None;
};