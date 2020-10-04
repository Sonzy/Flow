#pragma once

//= Includes ================================================

#include <string>
#include <unordered_map>
#include <memory>
#include "ConstantBuffers\DynamicConstantBuffer.h"

//= Class Definition ========================================

namespace DynamicCB
{
	class LayoutCodex
	{
	public:
		
		//= Public Static Functions =============================================

		static CookedLayout				Resolve(DynamicCB::RawLayout&& Layout);

	private:

		//= Private Functions ===================================================

		static LayoutCodex& Get();

	private:

		//= Private Variables ===================================================

		std::unordered_map<std::string, std::shared_ptr<DynamicCB::LayoutElement>>			m_Map;
	};
}