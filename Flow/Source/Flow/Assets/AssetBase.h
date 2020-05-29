#pragma once

enum class EAssetType
{
	None,
	Mesh,
	Texture,
	Shader
};

/* Base class for assets. Manages naming and asset types */
class FLOW_API AssetBase
{
public:

	virtual bool LoadAsset(const std::string& LocalPath);

	size_t GetAssetSize() const { return _AssetSize; }

	/* Returns a string with the asset size */
	std::string GetFormattedSize() const;

	void SetAssetName(const std::string& Name) { _AssetName = Name; }
	const std::string& GetAssetName() { return _AssetName; }

	EAssetType GetAssetType() { return _AssetType; }
protected:
	std::string _AssetName;
	size_t _AssetSize = 0;
	EAssetType _AssetType = EAssetType::None;
};