#pragma once

//= Includes ========================

#include <DirectXMath.h>
#include <d3d11.h>

//= Global Structs ==================

//TODO: Move somewhere appropriate
//4x8bit Colour 
struct RGBAColour
{
	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char a;

};

//= Global Enums ====================

enum class ElementType : unsigned char
{
	Position2D = 0b1,
	Position3D = 0b11,
	Texture2D = 0b111,
	Normal = 0b1111,
	Float3Colour = 0b11111,
	Float4Colour = 0b111111,
	RGBAColour = 0b1111111,
};

//= Global Structs Definitions ======

/* Describes the layout of a vertex, meta template programmed for ease of use */
template<ElementType>
struct VertexElement {};

template<>
struct VertexElement <ElementType::Position2D>
{
	using DataType = DirectX::XMFLOAT2;
	static constexpr DXGI_FORMAT Format = DXGI_FORMAT_R32G32_FLOAT;
	static constexpr const char* SemanticName = "Position";
	static constexpr const char* Code = "Pos2D";
};

template<>
struct VertexElement<ElementType::Position3D>
{
	using DataType = DirectX::XMFLOAT3;
	static constexpr DXGI_FORMAT Format = DXGI_FORMAT_R32G32B32_FLOAT;
	static constexpr const char* SemanticName = "Position";
	static constexpr const char* Code = "Pos3D";
};

template<>
struct VertexElement<ElementType::Texture2D>
{
	using DataType = DirectX::XMFLOAT2;
	static constexpr DXGI_FORMAT Format = DXGI_FORMAT_R32G32_FLOAT;
	static constexpr const char* SemanticName = "Texcoord";
	static constexpr const char* Code = "Tex2D";
};

template<>
struct VertexElement<ElementType::Normal>
{
	using DataType = DirectX::XMFLOAT3;
	static constexpr DXGI_FORMAT Format = DXGI_FORMAT_R32G32B32_FLOAT;
	static constexpr const char* SemanticName = "Normal";
	static constexpr const char* Code = "Norm";
};

template<>
struct VertexElement<ElementType::Float3Colour>
{
	using DataType = DirectX::XMFLOAT3;
	static constexpr DXGI_FORMAT Format = DXGI_FORMAT_R32G32B32_FLOAT;
	static constexpr const char* SemanticName = "Color";
	static constexpr const char* Code = "ColF3";
};

template<>
struct VertexElement<ElementType::Float4Colour>
{
	using DataType = DirectX::XMFLOAT4;
	static constexpr DXGI_FORMAT Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	static constexpr const char* SemanticName = "Color";
	static constexpr const char* Code = "ColF4";
};

template<>
struct VertexElement<ElementType::RGBAColour>
{
	using DataType = RGBAColour;
	static constexpr DXGI_FORMAT Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	static constexpr const char* SemanticName = "Color";
	static constexpr const char* Code = "Col32b";
};

//= Class Definition =========================

/* Single element of a vertex */
class Element
{
public:

	//= Public Functions =====================

	Element(ElementType Type, size_t Offset)
		: m_ElementType(Type)
		, m_Offset(Offset)
	{

	}

	size_t						GetOffset() const;		/* Gets the size offset for this element */
	size_t						GetOffsetAfter() const;	/* Gets the size offset for the next element */
	size_t						GetSize() const;
	ElementType					GetType() const;
	D3D11_INPUT_ELEMENT_DESC	GetDescription() const;	/* Translates the element to a DX11 element description */

	std::string					GetCode() const;

private:

	//= Private Static Functions =============

	static constexpr size_t GetSizeOf(ElementType Type);

	template<ElementType type>
	static constexpr D3D11_INPUT_ELEMENT_DESC GenerateDescription(size_t offset) noexcept
	{
		return { VertexElement<type>::SemanticName,0,VertexElement<type>::Format,0,(UINT)offset,D3D11_INPUT_PER_VERTEX_DATA,0 };
	}

private:

	//= Private Variables ====================

	ElementType					m_ElementType;
	size_t						m_Offset;
};