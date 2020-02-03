#pragma once
#include "VertexElement.h"
#include <vector>

namespace Flow
{
	class VertexLayout
	{
	public:
		/* Adds a new element to the layout */
		VertexLayout& Append(ElementType type);
		size_t GetSize() const;
		const Element& GetElementByIndex(size_t Index) const;
		size_t GetElementCount() const;

		/* Gets the layout as a vector of Input Element Descriptons */
		std::vector<D3D11_INPUT_ELEMENT_DESC> GetD3DLayout() const;


		/* Returns a const ref to the element in the layout*/
		template<typename ElementType Type>
		const Element& GetElement() const
		{
			for (auto& e : m_Elements)
			{
				if (e.GetType() == Type)
					return e;
			}

			FLOW_ENGINE_WARNING("VertexLayout::GetElement: Failed to get element type");
			return m_Elements.front();
		}

	private:
		std::vector<Element> m_Elements;
	};
}