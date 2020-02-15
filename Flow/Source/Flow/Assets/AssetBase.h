#pragma once

namespace Flow
{
	class AssetBase
	{
	public:

		virtual bool LoadAsset(const std::string& LocalPath);
	};
}