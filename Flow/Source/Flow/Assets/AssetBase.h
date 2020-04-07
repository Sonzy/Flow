#pragma once

namespace Flow
{
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

		size_t GetAssetSize() const { return AssetSize_; }

		/* Returns a string with the asset size */
		std::string GetFormattedSize() const;

		void SetAssetName(const std::string& Name) { AssetName_ = Name; }
		const std::string& GetAssetName() { return AssetName_; }

		EAssetType GetAssetType() { return AssetType_; }
	protected:
		std::string AssetName_;
		size_t AssetSize_ = 0;
		EAssetType AssetType_ = EAssetType::None;
	};
}