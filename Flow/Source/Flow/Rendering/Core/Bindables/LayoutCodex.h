#pragma once
#include "ConstantBuffers\DynamicConstantBuffer.h"
#include <string>
#include <unordered_map>
#include <memory>

namespace Flow
{
	namespace DCB
	{
		class LayoutCodex
		{
		public:
			static DCB::CookedLayout Resolve(DCB::RawLayout&& Layout);
		private:
			static LayoutCodex& Get();
			std::unordered_map<std::string, std::shared_ptr<DCB::LayoutElement>> Map_;
		};
	}
}