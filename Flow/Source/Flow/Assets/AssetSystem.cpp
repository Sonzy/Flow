#include "Flowpch.h"
#include "AssetSystem.h"

#include "Meshes\MeshAsset.h"
#include "Textures\TextureAsset.h"
#include "Shaders\ShaderAsset.h"
#include "Flow\Assets\Textures\GDIPlusManager.h"

#include "ThirdParty\ImGui\imgui.h"

namespace Flow
{
	AssetSystem* AssetSystem::s_AssetSystem = new AssetSystem();
	GDIPlusManager s_GDIPlusManager;

	AssetSystem::AssetSystem()
	{
	}

	void AssetSystem::Shutdown()
	{
		delete s_AssetSystem;
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

	bool AssetSystem::LoadAsset(const std::string& AssetName, const std::string& AssetPath)
	{
		AssetBase* NewAsset = CreateAsset(AssetSystem::GetAssetTypeFromFileExtension(AssetPath));
		CHECK_RETURN_FALSE(!NewAsset, "AssetSystem::LoadAsset: Failed to create asset for file type.");

		if (NewAsset->LoadAsset(AssetPath))
		{
			NewAsset->SetAssetName(AssetName);
			std::size_t HashedName = std::hash<std::string>{}(AssetName);
			s_AssetSystem->m_LoadedAssets.insert({ HashedName, NewAsset });

			s_AssetSystem->LoadedAssetSize += NewAsset->GetAssetSize();


			return true;
		}
		else
			FLOW_ENGINE_ERROR("AssetSystem::LoadAsset: Failed to load asset {0} at path {1}", AssetName, AssetPath);

		return false;
	}


	AssetBase* AssetSystem::GetAsset(const std::string& AssetName)
	{
		//Hash the string
		std::size_t HashedName = std::hash<std::string>{}(AssetName);

		//Error if the path doesnt exist in the system
		if (s_AssetSystem->m_LoadedAssets.find(HashedName) == s_AssetSystem->m_LoadedAssets.end())
		{
			FLOW_ENGINE_ERROR("AssetSystem::GetAsset: Tried to get asset from path ({0}) and failed", AssetName);
			return nullptr;
		}

		return s_AssetSystem->m_LoadedAssets[HashedName];
	}

	//TODO: Come up wiht a better version of this
	EAssetType AssetSystem::GetAssetTypeFromFileExtension(const std::string& AssetPath)
	{
		std::string Extension = AssetPath.substr(AssetPath.find_last_of('.') + 1);

		if (Extension._Equal("jpg") || Extension._Equal("png"))
			return EAssetType::Texture;

		if (Extension._Equal("obj") || Extension._Equal("FBX"))
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
			ImGui::Text("Asset Memory usage: %.1f MB", (float)s_AssetSystem->LoadedAssetSize / 1048576);

			if (ImGui::TreeNode("Meshes"))
			{
				for (auto Asset : s_AssetSystem->m_LoadedAssets)
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
				for (auto Asset : s_AssetSystem->m_LoadedAssets)
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
				for (auto Asset : s_AssetSystem->m_LoadedAssets)
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
}