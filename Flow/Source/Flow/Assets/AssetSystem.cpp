#include "Flowpch.h"
#include "AssetSystem.h"

#include "Meshes\MeshAsset.h"
#include "Textures\TextureAsset.h"
#include "Flow\Assets\Textures\GDIPlusManager.h"

namespace Flow
{
	AssetSystem* AssetSystem::s_AssetSystem = new AssetSystem();
	GDIPlusManager s_GDIPlusManager;

	AssetSystem::AssetSystem()
	{
	}

	AssetSystem::~AssetSystem()
	{
		//Unload all assets
		for (auto& Asset : m_LoadedAssets)
		{
			delete Asset.second;
		}
	}

	void AssetSystem::InitialiseAssetSystem()
	{

	}

	bool AssetSystem::LoadAsset(const std::string& AssetPath)
	{
		switch (AssetSystem::GetAssetTypeFromFileExtension(AssetPath))
		{
		case EAssetType::Mesh:
		{
			MeshAsset* NewMesh = new MeshAsset();
			//If we succeeded
			if (NewMesh->LoadAsset(AssetPath))
			{
				std::size_t HashedPath = std::hash<std::string>{}(AssetPath);
				s_AssetSystem->m_LoadedAssets.insert({ HashedPath, NewMesh });
				return true;
			}

			return false;
		}
		case EAssetType::Texture:
		{
			TextureAsset* Texture = new TextureAsset();
			//If we succeeded
			if (Texture->LoadAsset(AssetPath))
			{
				std::size_t HashedPath = std::hash<std::string>{}(AssetPath);
				s_AssetSystem->m_LoadedAssets.insert({ HashedPath, Texture });
				return true;
			}
			return false;
		}
		default:
			FLOW_ENGINE_ERROR("AssetSystem::LoadAsset: Failed to resolve asset type {0}", AssetPath);
			return false;
		}
	}


	AssetBase* AssetSystem::GetAsset(const std::string& AssetPath)
	{
		//Hash the string
		std::size_t HashedPath = std::hash<std::string>{}(AssetPath);

		//Error if the path doesnt exist in the system
		if (s_AssetSystem->m_LoadedAssets.find(HashedPath) == s_AssetSystem->m_LoadedAssets.end())
		{
			FLOW_ENGINE_ERROR("Tried to get asset from path ({0}) and failed", AssetPath);
			return nullptr;
		}

		return s_AssetSystem->m_LoadedAssets[HashedPath];
	}

	//TODO: Come up wiht a better version of this
	EAssetType AssetSystem::GetAssetTypeFromFileExtension(const std::string& AssetPath)
	{
		std::string Extension = AssetPath.substr(AssetPath.find_last_of('.') + 1);

		if (Extension._Equal("jpg") || Extension._Equal("png"))
			return EAssetType::Texture;

		if (Extension._Equal("obj"))
			return EAssetType::Mesh;
		
		return EAssetType::None;
	}
}