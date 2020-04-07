#include "Flowpch.h"
#include "AssetBase.h"


namespace Flow
{
	bool AssetBase::LoadAsset(const std::string& LocalPath)
	{
		FLOW_ENGINE_WARNING("AssetBase::LoadAsset: Load definition not found.");
		return false;
	}

	std::string AssetBase::GetFormattedSize() const
	{
		char Buffer[32];

		if (AssetSize_ > 1000000)
			sprintf_s(Buffer, "%.1f MB", (float)AssetSize_ / 1000000);
		else if (AssetSize_ > 1000)
			sprintf_s(Buffer, "%.1f KB", (float)AssetSize_ / 1000);
		else
			sprintf_s(Buffer, "%zu B", AssetSize_);

		return Buffer;
	}
}