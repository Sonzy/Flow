#include "pch.h"
#include "Application.h"
#include "AssetSystem.h"

#include "Meshes\MeshAsset.h"
#include "Textures\TextureAsset.h"
#include "Shaders\ShaderAsset.h"

#include "ThirdParty\ImGui\imgui.h"

#include "Typedefs.h"

#include <yaml-cpp/yaml.h>

AssetSystem* AssetSystem::sm_AssetSystem = new AssetSystem();

AssetSystem::AssetSystem()
	: m_LoadedAssets()
	, m_MemoryUsage(0)
{
}

AssetSystem::~AssetSystem()
{

}

bool AssetSystem::SaveAssetMap()
{
	//Generate the save data
	YAML::Emitter file;
	file << YAML::BeginMap;
	{
		file << YAML::Key << "ReferencedAssets";
		file << YAML::Value << YAML::BeginSeq;
		{
			//Save assets in order
			std::unordered_set<const char*> savedAssets;

			SaveAssetType<MeshAsset>(file, savedAssets, false);
			SaveAssetType<TextureAsset>(file, savedAssets, false);
			SaveAssetType<ShaderAsset>(file, savedAssets, false);

			for (std::pair<size_t, Asset*> asset : m_LoadedAssets)
			{
				if (savedAssets.find(asset.second->GetAssetName().c_str()) != savedAssets.end())
				{
					continue;
				}

				const Asset::MetaData& metadata = asset.second->GetMetaData();

				if (metadata.m_EditorAsset == false)
				{
					file << metadata.m_GamePath;
				}
			}
		}
		file << YAML::EndSeq;
	}
	file << YAML::EndMap;

	// Save the generated YAML data
	fs::path FilePath = GetEngineAssetDirectory().append("AssetMap.yaml");
	std::ofstream OutStream = std::ofstream(FilePath);
	OutStream << file.c_str();
	OutStream.close();

	return true;
}

bool AssetSystem::SaveEditorAssetMap()
{
	//Generate the save data
	YAML::Emitter file;
	file << YAML::BeginMap;
	{
		file << YAML::Key << "ReferencedAssets";
		file << YAML::Value << YAML::BeginSeq;
		{
			//Save assets in order
			std::unordered_set<const char*> savedAssets;

			SaveAssetType<MeshAsset>(file, savedAssets, true);
			SaveAssetType<TextureAsset>(file, savedAssets, true);
			SaveAssetType<ShaderAsset>(file, savedAssets, true);

			for (std::pair<size_t, Asset*> asset : m_LoadedAssets)
			{
				if (savedAssets.find(asset.second->GetAssetName().c_str()) != savedAssets.end())
				{
					continue;
				}

				const Asset::MetaData& metadata = asset.second->GetMetaData();

				if (metadata.m_EditorAsset == true)
				{
					file << metadata.m_GamePath;
				}
			}
		}
		file << YAML::EndSeq;
	}

	// Save the generated YAML data
	fs::path FilePath = GetEngineAssetDirectory().append("EditorAssetMap.yaml");
	std::ofstream OutStream = std::ofstream(FilePath);
	OutStream << file.c_str();
	OutStream.close();

	return true;
}

void AssetSystem::LoadAssetsFromAssetMap(const std::filesystem::path& filepath, bool Editor)
{
	std::ifstream InputStream = std::ifstream(filepath);
	if (InputStream.is_open() == false)
	{
		FLOW_ENGINE_LOG("AssetSystem::LoadAssetsFromAssetMap: Failed to open level save file");
		return;
	}

	//Load the whole file
	std::stringstream stream;
	stream << InputStream.rdbuf();

	//Get the YAML data
	YAML::Node data = YAML::Load(stream.str());

	if (data.IsDefined() == false)
	{
		FLOW_ENGINE_WARNING("AssetSystem::LoadAssetsFromAssetMap: Failed to load any data for %s", filepath.c_str());
		return;
	}

	auto assets = data["ReferencedAssets"];
	if (assets.IsDefined())
	{
		for (auto asset : assets)
		{
			std::string assetPath = asset.as<std::string>();

			//Skip commented out assets
			if (assetPath._Starts_with("#"))
			{
				continue;
			}

			LoadAsset(assetPath);
		}
	}
}

void AssetSystem::Startup()
{
	FLOW_ENGINE_LOG("Loading Editor Assets");
	sm_AssetSystem->LoadAssetsFromAssetMap(GetEngineAssetDirectory().append("EditorAssetMap.yaml"), true);
	FLOW_ENGINE_LOG("Loading Game Assets");
	sm_AssetSystem->LoadAssetsFromAssetMap(GetEngineAssetDirectory().append("AssetMap.yaml"), false);

	//TODO: Why did I do this originally?
	//sm_AssetSystem->SaveEditorAssetMap();
}

void AssetSystem::Shutdown()
{
	if (sm_AssetSystem)
	{
		sm_AssetSystem->m_LoadedAssets.clear();

		delete sm_AssetSystem;
	}
}

bool AssetSystem::HasExtension(const char* File, const char* Extension)
{
	return _stricmp(File, Extension) == 0;
}

bool AssetSystem::LoadAsset(const std::string& FilePath)
{
	//Create a new asset
	Asset* NewAsset = CreateAsset(AssetSystem::GetAssetTypeFromFileExtension(FilePath));
	CHECK_RETURN_FALSE(!NewAsset, "AssetSystem::LoadAsset: Failed to create asset for file type.");

	std::string FullPath = std::filesystem::current_path().parent_path().append(FilePath).string();
	if (NewAsset->LoadAsset(FullPath) == false)
	{
		FLOW_ENGINE_ERROR("AssetSystem::LoadAsset: Failed to load asset at path %s", FullPath.c_str());
		return false;
	}

	std::filesystem::path path_FilePath = FilePath;
	std::string FileName = path_FilePath.filename().replace_extension().string();
	std::size_t HashedName = std::hash<std::string>{}(FileName.c_str());//Use the c_str otherwise the null char is hashed too
	sm_AssetSystem->m_LoadedAssets.insert({ HashedName, NewAsset });

	//Update tracked data size
	sm_AssetSystem->m_MemoryUsage += NewAsset->GetAssetSize();

	//Keep track of it
	NewAsset->GetMetaData().m_IDHash = HashedName;
	NewAsset->GetMetaData().m_EditorAsset = true;
	NewAsset->GetMetaData().m_GamePath = FilePath;
	NewAsset->GetMetaData().m_OriginalPath = FilePath;//TODO: Need to save and load this to the fmesh
	NewAsset->SetAssetName(FileName);//TODO: Manage this properly

	if (bool SaveOnImport = true) //TODO: Make a setting?
	{
		sm_AssetSystem->SaveAssetMap();
	}

	return true;
}

bool AssetSystem::ImportAsset(const std::string& FilePath, const std::string& InputDirectory)
{
	//Create a new asset
	Asset* NewAsset = CreateAsset(AssetSystem::GetAssetTypeFromFileExtension(FilePath));
	CHECK_RETURN_FALSE(!NewAsset, "AssetSystem::LoadAsset: Failed to create asset for file type.");

	if (NewAsset->ImportAsset(FilePath, InputDirectory) == false)
	{
		FLOW_ENGINE_ERROR("AssetSystem::LoadAsset: Failed to load asset at path %s", FilePath.c_str());
		return false;
	}

	std::filesystem::path path_FilePath = InputDirectory;
	std::filesystem::path path_OriginalPath = InputDirectory;
	std::string FileName = path_FilePath.filename().replace_extension().string();
	std::size_t HashedName = std::hash<std::string>{}(FileName.c_str());//Use the c_str otherwise the null char is hashed too
	sm_AssetSystem->m_LoadedAssets.insert({ HashedName, NewAsset });

	//Keep track of it
	fs::path relativePath = fs::relative(path_OriginalPath, Application::GetEnginePath().parent_path());

	//Setup Metadata
	NewAsset->GetMetaData().m_IDHash = HashedName;
	NewAsset->GetMetaData().m_EditorAsset = true;
	NewAsset->GetMetaData().m_GamePath = relativePath.string();
	NewAsset->GetMetaData().m_OriginalPath = FilePath;
	NewAsset->SetAssetName(FileName);//TODO: Manage this properly

	//Update tracked data size
	sm_AssetSystem->m_MemoryUsage += NewAsset->GetAssetSize();

	if (bool SaveOnImport = true) //TODO: Make a setting?
	{
		sm_AssetSystem->SaveAssetMap();
		sm_AssetSystem->SaveEditorAssetMap();
	}

	return true;
}

bool AssetSystem::DoesAssetExist(const std::string& AssetPath)
{
	std::size_t HashedName = std::hash<std::string>{}(AssetPath.c_str()); //Use the c_str otherwise the null char is hashed too
	return sm_AssetSystem->m_LoadedAssets.find(HashedName) != sm_AssetSystem->m_LoadedAssets.end();
}

void AssetSystem::UpdateAssetName(const std::string& Old, const std::string& New)
{
	FilePath fpNew = New;
	FilePath fpOld = Old;
	Asset* FoundAsset = GetAsset(fpOld.stem().string());
	if (FoundAsset == nullptr)
	{
		FLOW_ENGINE_ERROR("Failed to update asset with name %s", Old.c_str());
		return;
	}

	//Update the asset identifier
	Asset::MetaData& metaData = FoundAsset->GetMetaData();
	metaData.m_GamePath = New;

	//Calculate the new hash
	auto hashData = sm_AssetSystem->m_LoadedAssets.extract(metaData.m_IDHash);
	hashData.key() = std::hash<std::string>{}(fpNew.stem().string().c_str());

	//Update it back into the asset map
	sm_AssetSystem->m_LoadedAssets.insert(std::move(hashData));

	//Save the asset map to disk
	sm_AssetSystem->SaveAssetMap();
}

Asset* AssetSystem::GetAsset(const std::string& AssetName)
{
	//Hash the string
	std::size_t HashedName = std::hash<std::string>{}(AssetName.c_str()); //Use the c_str otherwise the null char is hashed too

	//Error if the path doesnt exist in the system
	if (sm_AssetSystem->m_LoadedAssets.find(HashedName) == sm_AssetSystem->m_LoadedAssets.end())
	{
		FLOW_ENGINE_ERROR("AssetSystem::GetAsset: Tried to get asset from path (%s) and failed", AssetName.c_str());
		return nullptr;
	}

	return sm_AssetSystem->m_LoadedAssets[HashedName];
}

//TODO: Come up wiht a better version of this
Asset::Type AssetSystem::GetAssetTypeFromFileExtension(const std::string& AssetPath)
{
	fs::path FilePath = AssetPath;
	fs::path Extension = FilePath.extension();
	std::string ExtensionString = Extension.string();

	if (HasExtension(ExtensionString.c_str(), ".jpg") || HasExtension(ExtensionString.c_str(), ".png"))
	{
		return Asset::Type::Texture;
	}

	if (HasExtension(ExtensionString.c_str(), ".fmesh") || HasExtension(ExtensionString.c_str(), ".obj") || HasExtension(ExtensionString.c_str(), ".fbx"))
	{
		return Asset::Type::Mesh;
	}

	if (HasExtension(ExtensionString.c_str(), ".cso"))
	{
		return Asset::Type::Shader;
	}

	FLOW_ENGINE_ERROR("Failed to get extension for %s", Extension.string().c_str());
	return Asset::Type::None;
}

Asset* AssetSystem::CreateAsset(Asset::Type Type)
{
	switch (Type)
	{
	case Asset::Type::Mesh:
	{
		MeshAsset* NewMesh = new MeshAsset();
		return NewMesh;
	}
	case Asset::Type::Texture:
	{
		TextureAsset* Texture = new TextureAsset();
		return Texture;
	}
	case Asset::Type::Shader:
	{
		ShaderAsset* Shader = new ShaderAsset();
		return Shader;
	}
	default:
		FLOW_ENGINE_ERROR("AssetSystem::CreateAsset: Case Error");
		return nullptr;
	}
}

void AssetSystem::RenderDebugWindow(bool Render)
{
	if (ImGui::Begin("Asset System"))
	{
		ImGui::Text("Asset Memory usage: %.1f MB", static_cast<float>(sm_AssetSystem->m_MemoryUsage) / 1048576.0f);

		if (ImGui::TreeNode("Meshes"))
		{
			for (auto Asset : sm_AssetSystem->m_LoadedAssets)
			{
				//First: Name, Second: AssetPtr
				if (Asset.second->GetAssetType() != Asset::Type::Mesh)
					continue;

				ImGui::Text("%s: %s", Asset.second->GetAssetName().c_str(), Asset.second->GetFormattedSize().c_str());
			}
			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Textures"))
		{
			for (auto Asset : sm_AssetSystem->m_LoadedAssets)
			{
				//First: Name, Second: AssetPtr
				if (Asset.second->GetAssetType() != Asset::Type::Texture)
					continue;

				ImGui::Text("%s: %s", Asset.second->GetAssetName().c_str(), Asset.second->GetFormattedSize().c_str());
			}
			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Shaders"))
		{
			for (auto Asset : sm_AssetSystem->m_LoadedAssets)
			{
				//First: Name, Second: AssetPtr
				if (Asset.second->GetAssetType() != Asset::Type::Shader)
					continue;

				ImGui::Text("%s: %s", Asset.second->GetAssetName().c_str(), Asset.second->GetFormattedSize().c_str());
			}
			ImGui::TreePop();
		}

	}
	ImGui::End();
}

fs::path AssetSystem::GetEngineAssetDirectory()
{
	return Application::GetEnginePath().append("Assets");
}

fs::path AssetSystem::GetGameAssetDirectory()
{
	return Application::GetGamePath().append("Assets");
}

fs::path AssetSystem::GetEngineAssetParentDirectory()
{
	return Application::GetEnginePath();
}

fs::path AssetSystem::GetGameAssetParentDirectory()
{
	return Application::GetGamePath();
}
