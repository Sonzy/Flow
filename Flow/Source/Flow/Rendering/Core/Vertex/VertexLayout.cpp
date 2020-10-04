//= Includes ======================================================

#include "Flowpch.h"
#include "VertexLayout.h"

//= Class Definition - Vertex Layout ==============================

VertexLayout& VertexLayout::Append(ElementType type)
{
	m_Elements.emplace_back(type, GetSize());
	return *this;
}

size_t VertexLayout::GetSize() const
{
	return m_Elements.empty() ? 0u : m_Elements.back().GetOffsetAfter();
}

const Element& VertexLayout::GetElementByIndex(size_t Index) const
{
	return m_Elements[Index];
}

size_t VertexLayout::GetElementCount() const
{
	return m_Elements.size();
}

std::vector<D3D11_INPUT_ELEMENT_DESC> VertexLayout::GetD3DLayout() const
{
	std::vector<D3D11_INPUT_ELEMENT_DESC> VertexDescription;
	VertexDescription.reserve(GetElementCount());

	for (const auto& e : m_Elements)
	{
		VertexDescription.push_back(e.GetDescription());
	}
	return VertexDescription;
}

std::string VertexLayout::GetCode() const
{
	std::string Code;
	for (const auto& El : m_Elements)
	{
		Code += El.GetCode();
	}
	return Code;
}