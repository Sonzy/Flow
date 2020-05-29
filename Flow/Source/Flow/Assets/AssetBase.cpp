#include "Flowpch.h"
#include "AssetBase.h"


bool AssetBase::LoadAsset(const std::string& LocalPath)
{
	FLOW_ENGINE_WARNING("AssetBase::LoadAsset: Load definition not found.");
	return false;
}

std::string AssetBase::GetFormattedSize() const
{
	char Buffer[32];

	if (_AssetSize > 1000000)
		sprintf_s(Buffer, "%.1f MB", (float)_AssetSize / 1000000);
	else if (_AssetSize > 1000)
		sprintf_s(Buffer, "%.1f KB", (float)_AssetSize / 1000);
	else
		sprintf_s(Buffer, "%zu B", _AssetSize);

	return Buffer;
}