#include "Flowpch.h"
#include "VertexElement.h"

namespace Flow
{
	size_t Element::GetOffset() const
	{
		return m_Offset;
	}

	size_t Element::GetOffsetAfter() const
	{
		return m_Offset + GetSize();
	}

	size_t Element::GetSize() const
	{
		return GetSizeOf(m_Type);
	}

	ElementType Element::GetType() const
	{
		return m_Type;
	}

	D3D11_INPUT_ELEMENT_DESC Element::GetDescription() const
	{
		switch (m_Type)
		{
		case ElementType::Position2D:
			return GenerateDescription<ElementType::Position2D>(GetOffset());
		case ElementType::Position3D:
			return GenerateDescription<ElementType::Position3D>(GetOffset());
		case ElementType::Texture2D:
			return GenerateDescription<ElementType::Texture2D>(GetOffset());
		case ElementType::Normal:
			return GenerateDescription<ElementType::Normal>(GetOffset());
		case ElementType::Float3Colour:
			return GenerateDescription<ElementType::Float3Colour>(GetOffset());
		case ElementType::Float4Colour:
			return GenerateDescription<ElementType::Float4Colour>(GetOffset());
		case ElementType::RGBAColour:
			return GenerateDescription<ElementType::RGBAColour>(GetOffset());

		default:
			FLOW_ENGINE_ERROR("VertexElement::GetDescription: Case Error");
			return D3D11_INPUT_ELEMENT_DESC{};
		}
	}

	constexpr size_t Element::GetSizeOf(ElementType Type)
	{
		switch (Type)
		{
		case ElementType::Position2D:
			return sizeof(VertexElement<ElementType::Position2D>::DataType);
		case ElementType::Position3D:
			return sizeof(VertexElement<ElementType::Position3D>::DataType);
		case ElementType::Texture2D:
			return sizeof(VertexElement<ElementType::Texture2D>::DataType);
		case ElementType::Normal:
			return sizeof(VertexElement<ElementType::Normal>::DataType);
		case ElementType::Float3Colour:
			return sizeof(VertexElement<ElementType::Float3Colour>::DataType);
		case ElementType::Float4Colour:
			return sizeof(VertexElement<ElementType::Float4Colour>::DataType);
		case ElementType::RGBAColour:
			return sizeof(VertexElement<ElementType::RGBAColour>::DataType);
		default:
			FLOW_ENGINE_ERROR("VertexElement::GetSizeOf: Case Error");
			return size_t();
		}
	}
}


