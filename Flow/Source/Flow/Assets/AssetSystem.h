#pragma once
#include <vector>
#include <string>
#include <functional>

#include "Asset.h"
#include "Assets\Materials\MaterialAsset.h"
#include "Assets\Shaders\ShaderAsset.h"
#include "Assets\Textures\TextureAsset.h"
#include "Assets\Materials\MaterialAsset.h"
#include "Utils/FileSystem.h"

//TODO: Reload the collision meshes, might need to import

/* Asset management class used for managing all assets in the engine */
class FLOW_API AssetSystem
{
public:

	//= Public Static Functions =============

	static Asset*								CreateAsset(Asset::Type Type);
	/* Loads an asset from the specified path, and stores it in the system so it can be accessed by the name. Editor asset
	flag will search for the file from the default editor directory. */
	static bool									LoadAsset(const std::string& FilePath);
	static bool									ImportAsset(const std::string& FilePath, const std::string& ImportDirectory);
	//static bool								LoadEditorAsset(const std::string& AssetName, const std::string& FilePath, bool AbsolutePath = false);
	static Asset*								GetAsset(const std::string& AssetPath);
	static bool 								DoesAssetExist(const std::string& AssetPath);

	static void									UpdateAssetName(const std::string& Old, const std::string& New);

	template <typename T>
	static T*									GetAsset(const std::string& AssetPath)
	{
		return dynamic_cast<T*>(GetAsset(AssetPath));
	}

	static fs::path								GetEngineAssetDirectory();
	static fs::path								GetGameAssetDirectory();
	static fs::path								GetEngineAssetParentDirectory();
	static fs::path								GetGameAssetParentDirectory();

	static Asset::Type							GetAssetTypeFromFileExtension(const std::string& AssetPath);

	/* Renders an IMGUI window showing current asset system memory usage. */
	static void									RenderDebugWindow(bool Render);

	static void									Startup();
	static void									Shutdown();

	static bool									HasExtension(const char* File, const char* Extension);

	//= Public Static Template Functions =====================================================

	/* Temp: Used to create a new material at runtime, templated on the material class */
	template <typename T>
	static bool									CreateMaterial(const std::string& AssetName)
	{
		MaterialAsset* NewAsset = new MaterialAsset();
		NewAsset->CreateMaterial<T>();

		NewAsset->SetAssetName(AssetName);
		std::size_t HashedName = std::hash<std::string>{}(AssetName);
		sm_AssetSystem->m_LoadedAssets.insert({ HashedName, NewAsset });

		sm_AssetSystem->m_MemoryUsage += NewAsset->GetAssetSize();

		return (bool)NewAsset;
	}

	// Returns a vector of all assets of this type
	template <typename T>
	static std::vector<const char*>				BuildAssetList()
	{
		std::vector<const char*> OutList;

		for (std::pair<size_t, Asset*> asset : sm_AssetSystem->m_LoadedAssets)
		{
			if (typeid(*asset.second) == typeid(T))
			{
				OutList.push_back(asset.second->GetAssetName().c_str());
			}
		}

		return OutList;
	}


	// Returns a vector of all assets of this type
	template <typename T>
	static std::vector<const Asset*>				BuildAssetDataList()
	{
		std::vector<const char*> OutList;

		for (std::pair<size_t, Asset*> asset : sm_AssetSystem->m_LoadedAssets)
		{
			if (typeid(*asset.second) == typeid(T))
			{
				OutList.push_back(asset.second->GetAssetName().c_str());
			}
		}

		return OutList;
	}

private:
	//TODO: Remove
	friend class MenuBar;

	//= Private Functions ==============================

	AssetSystem();
	AssetSystem(const AssetSystem&) = delete;
	~AssetSystem();

	bool										SaveAssetMap();
	bool										SaveEditorAssetMap();
	void										LoadAssetsFromAssetMap(const std::filesystem::path& filepath, bool Editor);
	static AssetSystem&							Get()					{ return *sm_AssetSystem; };

private:
	//= Private Static Variables =========================
	static AssetSystem* sm_AssetSystem;

private:

	//= Private Variables ================================

	// Map of all loaded asset pointers as well as HashedFilePath to all loaded assets
	std::unordered_map<size_t, Asset*>			m_LoadedAssets;
	size_t										m_MemoryUsage;
};