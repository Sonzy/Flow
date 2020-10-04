#include "Flowpch.h"
#include "Asset.h"

bool Asset::LoadAsset(const std::string& LocalPath)
{
	FLOW_ENGINE_WARNING("Asset::LoadAsset: Load definition not found.");
	return false;
}

bool Asset::ImportAsset(const std::string& LocalPath)
{
	FLOW_ENGINE_WARNING("Asset::ImportAsset: Load definition not found.");
	return false;
}

bool Asset::SaveAsset(const std::string& AssetName)
{
	FLOW_ENGINE_WARNING("Asset::SaveAsset: Load definition not found.");
	return false;
}

std::string Asset::GetFormattedSize() const
{
	char Buffer[32];

	if (m_AssetSize > 1000000)
	{
		sprintf_s(Buffer, 32, "%.1f MB", static_cast<float>(m_AssetSize) / 1000000.0f);
	}
	else if (m_AssetSize > 1000)
	{
		sprintf_s(Buffer, 32, "%.1f KB", static_cast<float>(m_AssetSize) / 1000.0f);
	}
	else
	{
		sprintf_s(Buffer, 32, "%zu B", m_AssetSize);
	}

	return Buffer;
}