#include "Flowpch.h"
#include "VertexLayout.h"

namespace Flow
{
	VertexLayout& VertexLayout::Append(ElementType type)
	{
		Elements_.emplace_back(type, GetSize());
		return *this;
	}

	size_t VertexLayout::GetSize() const
	{
		return Elements_.empty() ? 0u : Elements_.back().GetOffsetAfter();
	}

	const Element& VertexLayout::GetElementByIndex(size_t Index) const
	{
		return Elements_[Index];
	}

	size_t VertexLayout::GetElementCount() const
	{
		return Elements_.size();
	}

	std::vector<D3D11_INPUT_ELEMENT_DESC> VertexLayout::GetD3DLayout() const
	{
		std::vector<D3D11_INPUT_ELEMENT_DESC> VertexDescription;
		VertexDescription.reserve(GetElementCount());

		for (const auto& e : Elements_)
		{
			VertexDescription.push_back(e.GetDescription());
		}
		return VertexDescription;
	}

	std::string VertexLayout::GetCode() const
	{
		std::string Code;
		for (const auto& El : Elements_)
		{
			Code += El.GetCode();
		}
		return Code;
	}
}

