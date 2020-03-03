#pragma once
#include <vector>
#include <string>
#include "AssetBase.h"
#include <functional>
#include "Flow\Assets\Materials\MaterialAsset.h"

namespace Flow
{
	class FLOW_API AssetSystem
	{
	public:

		AssetSystem();
		AssetSystem(const AssetSystem&) = delete;

		~AssetSystem();

		void InitialiseAssetSystem();

		static bool LoadAsset(const std::string& AssetName, const std::string& FilePath);


		template <typename T>
		static bool CreateMaterial(const std::string& AssetName)
		{
			MaterialAsset* NewAsset = new MaterialAsset();
			NewAsset->CreateMaterial<T>();

			NewAsset->SetAssetName(AssetName);
			std::size_t HashedName = std::hash<std::string>{}(AssetName);
			s_AssetSystem->m_LoadedAssets.insert({ HashedName, NewAsset });

			s_AssetSystem->LoadedAssetSize += NewAsset->GetAssetSize();

			return (bool)NewAsset;
		}


		//TODO: Setup asset retrieval
		static AssetBase* GetAsset(const std::string& AssetPath);

		static EAssetType GetAssetTypeFromFileExtension(const std::string& AssetPath);
		static AssetBase* CreateAsset(EAssetType Type);
		
		static void RenderDebugWindow(bool Render);

		template <typename T>
		static T* GetAsset(const std::string& AssetPath)
		{
			return dynamic_cast<T*>(GetAsset(AssetPath));
		}

	public:
		static AssetSystem* s_AssetSystem;

		// Map of all loaded asset pointers as well as HashedFilePath to all loaded assets
		std::unordered_map<size_t, AssetBase*> m_LoadedAssets;

		size_t LoadedAssetSize = 0;

	};
}