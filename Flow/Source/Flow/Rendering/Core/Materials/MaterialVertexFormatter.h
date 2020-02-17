#pragma once
#include <vector>

namespace Flow
{
	class MaterialVertexFormatter
	{
	public:

		template<typename T>
		void EmplaceBack(T Type)
		{

		}

	private:
		std::vector<char> m_Buffer;
	};
}