#pragma once
#include <vector>
#include <string>
#include <functional>
#include <filesystem>

#include "Asset.h"
#include "Flow\Assets\Materials\MaterialAsset.h"
#include "Flow\Assets\Shaders\ShaderAsset.h"
#include "Flow\Assets\Textures\TextureAsset.h"
#include "Flow\Assets\Materials\MaterialAsset.h"


/* Asset management class used for managing all assets in the engine */
class FLOW_API AssetSystem
{
public:

	//= Public Static Functions =============

	static Asset*								CreateAsset(Asset::Type Type);
	/* Loads an asset from the specified path, and stores it in the system so it can be accessed by the name. Editor asset
	flag will search for the file from the default editor directory. */
	static bool									LoadAsset(const std::string& AssetName, const std::string& FilePath, bool EditorAsset = false, bool AbsolutePath = false);
	static bool									LoadEditorAsset(const std::string& AssetName, const std::string& FilePath, bool AbsolutePath = false);
	static Asset*								GetAsset(const std::string& AssetPath);

	template <typename T>
	static T*									GetAsset(const std::string& AssetPath)
	{
		return dynamic_cast<T*>(GetAsset(AssetPath));
	}

	static std::filesystem::path				GetRootDirectory(bool EditorPath = false);
	static std::string							GetRootDirectoryString(bool EditorPath = false);

	static std::filesystem::path				GetAssetDirectory(bool EditorPath = false);
	static std::string							GetAssetDirectoryString(bool EditorPath = false);

	static Asset::Type							GetAssetTypeFromFileExtension(const std::string& AssetPath);

	/* Renders an IMGUI window showing current asset system memory usage. */
	static void									RenderDebugWindow(bool Render);

	static void									Shutdown();

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

private:

	//= Private Functions ==============================

	AssetSystem();
	AssetSystem(const AssetSystem&) = delete;
	~AssetSystem();

private:
	//= Private Static Variables =========================
	static AssetSystem* sm_AssetSystem;

private:

	//= Private Variables ================================

	// Map of all loaded asset pointers as well as HashedFilePath to all loaded assets
	std::unordered_map<size_t, Asset*>			m_LoadedAssets;
	size_t										m_MemoryUsage;
};