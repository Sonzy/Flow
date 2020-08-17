#include "Flowpch.h"
#include "AssetSystem.h"

#include "Meshes\MeshAsset.h"
#include "Textures\TextureAsset.h"
#include "Shaders\ShaderAsset.h"

#include "ThirdParty\ImGui\imgui.h"

AssetSystem* AssetSystem::AssetSystem_s = new AssetSystem();

AssetSystem::AssetSystem()
{
}

void AssetSystem::Shutdown()
{
	delete AssetSystem_s;
}

AssetSystem::~AssetSystem()
{
	//Unload all assets
	for (auto& Asset : LoadedAssets_)
	{
		delete Asset.second;
	}
}

void AssetSystem::InitialiseAssetSystem()
{

}

bool AssetSystem::LoadAsset(const std::string& AssetName, const std::string& AssetPath, bool EditorAsset, bool AbsolutePath)
{
	//Create a new asset
	AssetBase* NewAsset = CreateAsset(AssetSystem::GetAssetTypeFromFileExtension(AssetPath));
	CHECK_RETURN_FALSE(!NewAsset, "AssetSystem::LoadAsset: Failed to create asset for file type.");

	std::string UpdatedPath = AbsolutePath ? AssetPath : GetAssetDirectoryString(EditorAsset) + AssetPath;
	//Try to load asset, log if we fail
	if (!NewAsset->LoadAsset(UpdatedPath))
	{
		FLOW_ENGINE_ERROR("AssetSystem::LoadAsset: Failed to load asset {0} at path {1}", AssetName, UpdatedPath);
		delete NewAsset;
		return false;
	}

	//Add the asset to the stored assets
	NewAsset->SetAssetName(AssetName);
	std::size_t HashedName = std::hash<std::string>{}(AssetName.c_str());//Use the c_str otherwise the null char is hashed too
	AssetSystem_s->LoadedAssets_.insert({ HashedName, NewAsset });

	//Update tracked data size
	AssetSystem_s->LoadedAssetSize_ += NewAsset->GetAssetSize();

	return true;
}

bool AssetSystem::LoadEditorAsset(const std::string& AssetName, const std::string& FilePath, bool AbsolutePath)
{
	return LoadAsset(AssetName, FilePath, true);
}


AssetBase* AssetSystem::GetAsset(const std::string& AssetName)
{
	//Hash the string
	std::size_t HashedName = std::hash<std::string>{}(AssetName.c_str()); //Use the c_str otherwise the null char is hashed too

	//Error if the path doesnt exist in the system
	if (AssetSystem_s->LoadedAssets_.find(HashedName) == AssetSystem_s->LoadedAssets_.end())
	{
		FLOW_ENGINE_ERROR("AssetSystem::GetAsset: Tried to get asset from path ({0}) and failed", AssetName.c_str());
		return nullptr;
	}

	return AssetSystem_s->LoadedAssets_[HashedName];
}

//TODO: Come up wiht a better version of this
EAssetType AssetSystem::GetAssetTypeFromFileExtension(const std::string& AssetPath)
{
	std::string Extension = AssetPath.substr(AssetPath.find_last_of('.') + 1);

	if (Extension._Equal("jpg") || Extension._Equal("png"))
		return EAssetType::Texture;

	if (Extension._Equal("obj") || Extension._Equal("fbx"))
		return EAssetType::Mesh;

	if (Extension._Equal("cso"))
		return EAssetType::Shader;

	return EAssetType::None;
}

AssetBase* AssetSystem::CreateAsset(EAssetType Type)
{
	switch (Type)
	{
	case EAssetType::Mesh:
	{
		MeshAsset* NewMesh = new MeshAsset();
		return NewMesh;
	}
	case EAssetType::Texture:
	{
		TextureAsset* Texture = new TextureAsset();
		return Texture;
	}
	case EAssetType::Shader:
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
		ImGui::Text("Asset Memory usage: %.1f MB", (float)AssetSystem_s->LoadedAssetSize_ / 1048576);

		if (ImGui::TreeNode("Meshes"))
		{
			for (auto Asset : AssetSystem_s->LoadedAssets_)
			{
				//First: Name, Second: AssetPtr
				if (Asset.second->GetAssetType() != EAssetType::Mesh)
					continue;

				ImGui::Text("%s: %s", Asset.second->GetAssetName().c_str(), Asset.second->GetFormattedSize().c_str());
			}
			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Textures"))
		{
			for (auto Asset : AssetSystem_s->LoadedAssets_)
			{
				//First: Name, Second: AssetPtr
				if (Asset.second->GetAssetType() != EAssetType::Texture)
					continue;

				ImGui::Text("%s: %s", Asset.second->GetAssetName().c_str(), Asset.second->GetFormattedSize().c_str());
			}
			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Shaders"))
		{
			for (auto Asset : AssetSystem_s->LoadedAssets_)
			{
				//First: Name, Second: AssetPtr
				if (Asset.second->GetAssetType() != EAssetType::Shader)
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
