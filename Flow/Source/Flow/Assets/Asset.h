#pragma once
#include <string>

//= Forward Declarations ==============================

namespace Bindables
{
	class Texture;
}

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

	enum class Origin
	{
		Editor,
		Game
	};

	//= Public Struct ==================================

	struct MetaData
	{
		std::string		m_OriginalPath;
		std::string		m_GamePath;
		std::size_t		m_IDHash;
		bool			m_EditorAsset;
	};

	//= Public Functions ===============================

	virtual bool			ImportAsset(const std::string& FilePath, const std::string& SavePath);
	virtual bool			SaveAsset(const std::string& AssetName);
	virtual bool			LoadAsset(const std::string& AssetName);

	void					SetAssetName(const std::string& Name)			{ m_AssetName = Name; }
	const std::string&		GetAssetName() const							{ return m_AssetName; }
	Asset::Type				GetAssetType() const							{ return m_AssetType; }
	size_t					GetAssetSize() const							{ return m_AssetSize; };
	// Returns a string with the asset size
	std::string				GetFormattedSize() const;

	const Asset::MetaData&	GetMetaData() const								{ return m_Metadata; }
	Asset::MetaData&		GetMetaData() 									{ return m_Metadata; }

protected:

	//= Protected Variables ===================================

	virtual void					CreateThumbnail() {};

protected:

	//= Protected Functions ===================================
	std::string				m_AssetName;
	size_t					m_AssetSize = 0;
	Asset::Type				m_AssetType = Asset::Type::None;
	Asset::MetaData			m_Metadata;
	Bindables::Texture*		m_Thumbnail = nullptr;
};