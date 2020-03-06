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

	class FLOW_API AssetBase
	{
	public:

		virtual bool LoadAsset(const std::string& LocalPath);
		size_t GetAssetSize() const { return m_AssetSize;	}

		/* */
		std::string GetFormattedSize() const;

		void SetAssetName(const std::string& Name) { m_AssetName = Name; }
		const std::string& GetAssetName() { return m_AssetName; }

		EAssetType GetAssetType() { return m_AssetType; }
	protected:
		std::string m_AssetName;
		size_t m_AssetSize = 0;
		EAssetType m_AssetType = EAssetType::None;
	};
}