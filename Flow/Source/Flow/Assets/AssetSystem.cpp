#include "Flowpch.h"
#include "AssetSystem.h"

#include "Meshes\MeshAsset.h"
#include "Textures\TextureAsset.h"
#include "Shaders\ShaderAsset.h"

#include "ThirdParty\ImGui\imgui.h"

AssetSystem* AssetSystem::sm_AssetSystem = new AssetSystem();

AssetSystem::AssetSystem()
	: m_LoadedAssets()
	, m_MemoryUsage(0)
{
}

AssetSystem::~AssetSystem()
{

}

void AssetSystem::Shutdown()
{
	if (sm_AssetSystem)
	{
		sm_AssetSystem->m_LoadedAssets.clear();

		delete sm_AssetSystem;
	}
}

bool AssetSystem::LoadAsset(const std::string& AssetName, const std::string& AssetPath, bool EditorAsset, bool AbsolutePath)
{
	//Create a new asset
	Asset* NewAsset = CreateAsset(AssetSystem::GetAssetTypeFromFileExtension(AssetPath));
	CHECK_RETURN_FALSE(!NewAsset, "AssetSystem::LoadAsset: Failed to create asset for file type.");

	std::string UpdatedPath = AbsolutePath ? AssetPath : GetAssetDirectoryString(EditorAsset) + AssetPath;
	//Try to load asset, log if we fail
	if (!NewAsset->ImportAsset(UpdatedPath)) //TODO: Load the asset instead
	{
		FLOW_ENGINE_ERROR("AssetSystem::LoadAsset: Failed to load asset {0} at path {1}", AssetName, UpdatedPath);
		delete NewAsset;
		return false;
	}

	//Add the asset to the stored assets
	NewAsset->SetAssetName(AssetName);
	std::size_t HashedName = std::hash<std::string>{}(AssetName.c_str());//Use the c_str otherwise the null char is hashed too
	sm_AssetSystem->m_LoadedAssets.insert({ HashedName, NewAsset });

	//Update tracked data size
	sm_AssetSystem->m_MemoryUsage += NewAsset->GetAssetSize();

	return true;
}

bool AssetSystem::LoadEditorAsset(const std::string& AssetName, const std::string& FilePath, bool AbsolutePath)
{
	return LoadAsset(AssetName, FilePath, true);
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

	if (Extension._Equal("obj") || Extension._Equal("fbx"))
		return Asset::Type::Mesh;

	if (Extension._Equal("cso"))
		return Asset::Type::Shader;

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
