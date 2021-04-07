#pragma once

//= Includes ============================

#include <vector>
#include "Framework/Logging/Log.h"
#include "VertexElement.h"

//= Class Definition ====================

class VertexLayout
{
public:

	//= Public Functions ======================================


	/* Adds a new element to the layout */
	VertexLayout&							Append(ElementType type);
	size_t									GetSize() const;
	const Element&							GetElementByIndex(size_t Index) const;
	size_t									GetElementCount() const;

	/* Gets the layout as a vector of Input Element Descriptons */
	std::vector<D3D11_INPUT_ELEMENT_DESC>	GetD3DLayout() const;
	std::string								GetCode() const;

	//= Public Template Functions =============================

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

	//= Private Variables =====================================

	std::vector<Element> m_Elements;
};