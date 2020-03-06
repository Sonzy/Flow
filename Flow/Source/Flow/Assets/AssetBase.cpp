#include "Flowpch.h"
#include "AssetBase.h"


namespace Flow
{
	bool AssetBase::LoadAsset(const std::string& LocalPath)
	{
		return false;
	}

	std::string AssetBase::GetFormattedSize() const
	{
		char Buffer[32];

		if (m_AssetSize > 1000000)
			sprintf_s(Buffer, "%.1f MB", (float)m_AssetSize / 1000000);
		else if (m_AssetSize > 1000)
			sprintf_s(Buffer, "%.1f KB", (float)m_AssetSize / 1000);
		else
			sprintf_s(Buffer, "%zu B", m_AssetSize);

		return Buffer;
	}
}

