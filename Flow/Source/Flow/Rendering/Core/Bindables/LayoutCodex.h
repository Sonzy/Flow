#pragma once
#include "ConstantBuffers\DynamicConstantBuffer.h"
#include <string>
#include <unordered_map>
#include <memory>

namespace DynamicCB
{
	class LayoutCodex
	{
	public:
		static CookedLayout Resolve(DynamicCB::RawLayout&& Layout);
	private:
		static LayoutCodex& Get();
		std::unordered_map<std::string, std::shared_ptr<DynamicCB::LayoutElement>> Map_;
	};
}