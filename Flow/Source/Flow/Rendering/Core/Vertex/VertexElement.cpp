#include "Flowpch.h"
#include "VertexElement.h"

size_t Element::GetOffset() const
{
	return Offset_;
}

size_t Element::GetOffsetAfter() const
{
	return Offset_ + GetSize();
}

size_t Element::GetSize() const
{
	return GetSizeOf(ElementType_);
}

ElementType Element::GetType() const
{
	return ElementType_;
}

D3D11_INPUT_ELEMENT_DESC Element::GetDescription() const
{
	switch (ElementType_)
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

std::string Element::GetCode() const
{
	switch (ElementType_)
	{
	case ElementType::Position2D:
		return VertexElement<ElementType::Position2D>::Code;
	case ElementType::Position3D:
		return VertexElement<ElementType::Position3D>::Code;
	case ElementType::Texture2D:
		return VertexElement<ElementType::Texture2D>::Code;
	case ElementType::Normal:
		return VertexElement<ElementType::Normal>::Code;
	case ElementType::Float3Colour:
		return VertexElement<ElementType::Float3Colour>::Code;
	case ElementType::Float4Colour:
		return VertexElement<ElementType::Float4Colour>::Code;
	case ElementType::RGBAColour:
		return VertexElement<ElementType::RGBAColour>::Code;
	default:
		FLOW_ENGINE_ERROR("VertexElement::GetCode: Case Error");
		return "nullptr";
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