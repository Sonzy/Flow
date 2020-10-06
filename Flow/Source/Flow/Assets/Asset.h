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

	enum class Origin
	{
		Editor,
		Game
	};

	//= Public Struct ==================================

	struct MetaData
	{
		std::string		m_AssetPath;
		bool			m_EditorAsset;
	};

	//= Public Functions ===============================

	virtual bool			ImportAsset(const std::string& LocalPath);
	virtual bool			SaveAsset(const std::string& AssetName);
	virtual bool			LoadAsset(const std::string& AssetName);

	void					SetEditorAsset(bool EditorAsset)				{ m_Metadata.m_EditorAsset = EditorAsset; }
	void					SetAssetPath(const std::string& Path)			{ m_Metadata.m_AssetPath = Path; }

	void					SetAssetName(const std::string& Name)			{ m_AssetName = Name; }
	const std::string&		GetAssetName() const							{ return m_AssetName; }
	Asset::Type				GetAssetType() const							{ return m_AssetType; }
	size_t					GetAssetSize() const							{ return m_AssetSize; };
	// Returns a string with the asset size
	std::string				GetFormattedSize() const;

	const Asset::MetaData& GetMetaData() const								{ return m_Metadata; }
protected:

	//= Protected Functions ==========================
	std::string				m_AssetName;
	size_t					m_AssetSize = 0;
	Asset::Type				m_AssetType = Asset::Type::None;
	Asset::MetaData			m_Metadata;
};