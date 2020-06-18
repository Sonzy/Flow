#pragma once
#include <vector>
#include <string>
#include "AssetBase.h"
#include <functional>

#include "Flow\Assets\Materials\MaterialAsset.h"
#include "Flow\Assets\Shaders\ShaderAsset.h"
#include "Flow\Assets\Textures\TextureAsset.h"
#include "Flow\Assets\Materials\MaterialAsset.h"

#include <filesystem>

/* Asset management class used for managing all assets in the engine */
class FLOW_API AssetSystem
{
public:

	AssetSystem();
	AssetSystem(const AssetSystem&) = delete;
	static void Shutdown();

	~AssetSystem();

	void InitialiseAssetSystem();

	/* Loads an asset from the specified path, and stores it in the system so it can be accessed by the name. Editor asset
	flag will search for the file from the default editor directory. */
	static bool LoadAsset(const std::string& AssetName, const std::string& FilePath, bool EditorAsset = false, bool AbsolutePath = false);
	static bool LoadEditorAsset(const std::string& AssetName, const std::string& FilePath, bool AbsolutePath = false);

	/* Temp: Used to create a new material at runtime, templated on the material class */
	template <typename T>
	static bool CreateMaterial(const std::string& AssetName)
	{
		MaterialAsset* NewAsset = new MaterialAsset();
		NewAsset->CreateMaterial<T>();

		NewAsset->SetAssetName(AssetName);
		std::size_t HashedName = std::hash<std::string>{}(AssetName);
		AssetSystem_s->LoadedAssets_.insert({ HashedName, NewAsset });

		AssetSystem_s->LoadedAssetSize_ += NewAsset->GetAssetSize();

		return (bool)NewAsset;
	}


	//TODO: Setup asset retrieval
	static AssetBase* GetAsset(const std::string& AssetPath);

	static EAssetType GetAssetTypeFromFileExtension(const std::string& AssetPath);
	static AssetBase* CreateAsset(EAssetType Type);

	/* Renders an IMGUI window showing current asset system memory usage. */
	static void RenderDebugWindow(bool Render);

	template <typename T>
	static T* GetAsset(const std::string& AssetPath)
	{
		return dynamic_cast<T*>(GetAsset(AssetPath));
	}

public:
	static AssetSystem* AssetSystem_s;

	// Map of all loaded asset pointers as well as HashedFilePath to all loaded assets
	std::unordered_map<size_t, AssetBase*> LoadedAssets_;

	size_t LoadedAssetSize_ = 0;

public:

	static std::filesystem::path GetRootDirectory(bool EditorPath = false);
	static std::string GetRootDirectoryString(bool EditorPath = false);

	static std::filesystem::path GetAssetDirectory(bool EditorPath = false);
	static std::string GetAssetDirectoryString(bool EditorPath = false);
};