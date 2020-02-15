#pragma once
#include <vector>
#include <string>
#include "AssetBase.h"
#include <functional>

namespace Flow
{
	enum class EAssetType
	{
		None,
		Mesh,
		Texture
	};


	class AssetSystem
	{
	public:

		AssetSystem();
		AssetSystem(const AssetSystem&) = delete;

		~AssetSystem();

		void InitialiseAssetSystem();

		static bool LoadAsset(const std::string& FilePath);

		//TODO: Setup asset retrieval
		static AssetBase* GetAsset(const std::string& AssetPath);

		static EAssetType GetAssetTypeFromFileExtension(const std::string& AssetPath);
		

		template <typename T>
		static T* GetAsset(const std::string& AssetPath)
		{
			return dynamic_cast<T*>(GetAsset(AssetPath));
		}

	public:

		// Map of all loaded asset pointers as well as HashedFilePath to all loaded assets
		std::unordered_map<size_t, AssetBase*> m_LoadedAssets;
		std::unordered_map<std::string, std::function<bool(const std::string&)>> m_AssetLoaders;
		static AssetSystem* s_AssetSystem;
	};
}