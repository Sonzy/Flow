#include "Flowpch.h"
#include "AssetSystem.h"

#include "Meshes\MeshAsset.h"
#include "Textures\TextureAsset.h"
#include "Shaders\ShaderAsset.h"

#include "ThirdParty\ImGui\imgui.h"

#include "Typedefs.h"

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
	std::ofstream OutStream = std::ofstream(
		std::filesystem::current_path().parent_path().append("Flow").string() + "/Assets/AssetMap.txt"
		, std::ios::out | std::ios::trunc);

	if (OutStream.is_open() == false)
	{
		FLOW_ENGINE_LOG("Failed to save asset map");
		return false;
	}

	uint32 Counter = 0;
	for (std::pair<size_t, Asset*> asset : m_LoadedAssets)
	{
		if (asset.second->GetMetaData().m_EditorAsset == false)
		{
			Counter++;
		}
	}

	//Number of paths
	OutStream << Counter;
	OutStream << "\n";

	for (std::pair<size_t, Asset*> asset : m_LoadedAssets)
	{
		const Asset::MetaData& metadata = asset.second->GetMetaData();

		if (metadata.m_EditorAsset == false)
		{
			OutStream << metadata.m_AssetPath;
			OutStream << "\n";
		}
	}

	return true;
}

void AssetSystem::LoadAssetsFromAssetMap(const std::filesystem::path& filepath, bool Editor)
{
	std::ifstream InputStream = std::ifstream(filepath, std::ios::in);

	if (InputStream.is_open() == false)
	{
		FLOW_ENGINE_LOG("Failed to open asset map");
		return;
	}

	size_t AssetCount;
	InputStream >> AssetCount;
	InputStream.seekg(1, std::ios::cur); //skip '\n'

	FLOW_ENGINE_LOG("AssetMapCount: {0}", AssetCount);


	for (size_t it = 0; it < AssetCount; it++)
	{
		char buffer[256] = { '\0' };
		InputStream.getline(buffer, 256, '\n');

		//Skip commented out assets
		if (buffer[0] == '#')
		{
			continue;
		}

		LoadAsset(buffer);
		FLOW_ENGINE_LOG("Loading to asset map: {0}", buffer);
	}
}

void AssetSystem::Startup()
{
	sm_AssetSystem->LoadAssetsFromAssetMap(std::filesystem::current_path().parent_path().append("Flow").string() + "/Assets/EditorAssetMap.txt", true);
	sm_AssetSystem->LoadAssetsFromAssetMap(std::filesystem::current_path().parent_path().append("Flow").string() + "/Assets/AssetMap.txt", false);
}

void AssetSystem::Shutdown()
{
	if (sm_AssetSystem)
	{
		sm_AssetSystem->m_LoadedAssets.clear();

		delete sm_AssetSystem;
	}
}

bool AssetSystem::LoadAsset(const std::string& FilePath)
{
	//Create a new asset
	Asset* NewAsset = CreateAsset(AssetSystem::GetAssetTypeFromFileExtension(FilePath));
	CHECK_RETURN_FALSE(!NewAsset, "AssetSystem::LoadAsset: Failed to create asset for file type.");

	std::string FullPath = std::filesystem::current_path().parent_path().append(FilePath).string();
	if (NewAsset->LoadAsset(FullPath) == false)
	{
		FLOW_ENGINE_ERROR("AssetSystem::LoadAsset: Failed to load asset at path {0}", FullPath);
		return false;
	}

	std::filesystem::path path_FilePath = FilePath;
	std::string FileName = path_FilePath.filename().replace_extension().string();
	std::size_t HashedName = std::hash<std::string>{}(FileName.c_str());//Use the c_str otherwise the null char is hashed too
	sm_AssetSystem->m_LoadedAssets.insert({ HashedName, NewAsset });

	//Update tracked data size
	sm_AssetSystem->m_MemoryUsage += NewAsset->GetAssetSize();

	//Keep track of it
	NewAsset->SetAssetPath(FilePath);
	NewAsset->SetEditorAsset(true);//TODO: Manage this properly
	NewAsset->SetAssetName(FileName);//TODO: Manage this properly

	const bool SaveOnImport = true;
	if (SaveOnImport == true)
	{
		sm_AssetSystem->SaveAssetMap();
	}

	return true;
}

bool AssetSystem::ImportAsset(const std::string& FilePath)
{
	//Create a new asset
	Asset* NewAsset = CreateAsset(AssetSystem::GetAssetTypeFromFileExtension(FilePath));
	CHECK_RETURN_FALSE(!NewAsset, "AssetSystem::LoadAsset: Failed to create asset for file type.");

	std::string FullPath = std::filesystem::current_path().parent_path().append(FilePath).string();
	if (NewAsset->ImportAsset(FullPath) == false)
	{
		FLOW_ENGINE_ERROR("AssetSystem::LoadAsset: Failed to load asset at path {0}", FullPath);
		return false;
	}

	std::filesystem::path path_FilePath = FilePath;
	std::string FileName = path_FilePath.filename().replace_extension().string();
	std::size_t HashedName = std::hash<std::string>{}(FileName.c_str());//Use the c_str otherwise the null char is hashed too
	sm_AssetSystem->m_LoadedAssets.insert({ HashedName, NewAsset });

	//Keep track of it
	NewAsset->SetAssetPath(FilePath);
	NewAsset->SetEditorAsset(true);//TODO: Manage this properly
	NewAsset->SetAssetName(FileName);//TODO: Manage this properly

	//Update tracked data size
	sm_AssetSystem->m_MemoryUsage += NewAsset->GetAssetSize();

	const bool SaveOnImport = true;
	if (SaveOnImport == true)
	{
		sm_AssetSystem->SaveAssetMap();
	}

	return true;
}

Asset* AssetSystem::GetAsset(const std::string& AssetName)
{
	//Hash the string
	std::size_t HashedName = std::hash<std::string>{}(AssetName.c_str()); //Use the c_str otherwise the null char is hashed too

	//Error if the path doesnt exist in the system
	if (sm_AssetSystem->m_LoadedAssets.find(HashedName) == sm_AssetSystem->m_LoadedAssets.end())
	{
		FLOW_ENGINE_ERROR("AssetSystem::GetAsset: Tried to get asset from path ({0}) and failed", AssetName.c_str());
		return nullptr;
	}

	return sm_AssetSystem->m_LoadedAssets[HashedName];
}

//TODO: Come up wiht a better version of this
Asset::Type AssetSystem::GetAssetTypeFromFileExtension(const std::string& AssetPath)
{
	std::string Extension = AssetPath.substr(AssetPath.find_last_of('.') + 1);

	if (Extension._Equal("jpg") || Extension._Equal("png"))
		return Asset::Type::Texture;

	if (Extension._Equal("fmesh") || Extension._Equal("obj") || Extension._Equal("fbx"))
		return Asset::Type::Mesh;

	if (Extension._Equal("cso"))
		return Asset::Type::Shader;

	FLOW_ENGINE_ERROR("Failed to get extension for {0}", Extension);
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

std::filesystem::path AssetSystem::GetRootDirectory(bool EditorPath)
{
	return EditorPath ? std::filesystem::current_path().parent_path().append("Flow") : std::filesystem::current_path();
}

std::string AssetSystem::GetRootDirectoryString(bool EditorPath)
{	
	return EditorPath ? std::filesystem::current_path().parent_path().append("Flow").string() + "/" : std::filesystem::current_path().string() + "/";
}

std::filesystem::path AssetSystem::GetAssetDirectory(bool EditorPath)
{
	return EditorPath ? std::filesystem::current_path().parent_path().append("Flow") : std::filesystem::current_path();
}

std::string AssetSystem::GetAssetDirectoryString(bool EditorPath)
{
	return EditorPath ? std::filesystem::current_path().parent_path().append("Flow").string() + "/" : std::filesystem::current_path().string() + "/";
}
