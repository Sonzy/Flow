#pragma once
#include <DirectXMath.h>
#include <d3d11.h>

namespace Flow
{
	//TODO: Move somewhere appropriate
	//4x8bit Colour 
	struct RGBAColour
	{
		unsigned char r;
		unsigned char g;
		unsigned char b;
		unsigned char a;

	};

	enum class ElementType
	{
		Position2D,
		Position3D,
		Texture2D,
		Normal,
		Float3Colour,
		Float4Colour,
		RGBAColour,
	};

	/* Describes the layout of a vertex, meta template programmed for ease of use */
	template<ElementType>
	struct VertexElement {};

	template<>
	struct VertexElement <ElementType::Position2D>
	{
		using DataType = DirectX::XMFLOAT2;
		static constexpr DXGI_FORMAT Format = DXGI_FORMAT_R32G32_FLOAT;
		static constexpr const char* SemanticName = "Position";
	};

	template<>
	struct VertexElement<ElementType::Position3D>
	{
		using DataType = DirectX::XMFLOAT3;
		static constexpr DXGI_FORMAT Format = DXGI_FORMAT_R32G32B32_FLOAT;
		static constexpr const char* SemanticName = "Position";
	};

	template<>
	struct VertexElement<ElementType::Texture2D>
	{
		using DataType = DirectX::XMFLOAT2;
		static constexpr DXGI_FORMAT Format = DXGI_FORMAT_R32G32_FLOAT;
		static constexpr const char* SemanticName = "Texcoord";
	};

	template<>
	struct VertexElement<ElementType::Normal>
	{
		using DataType = DirectX::XMFLOAT3;
		static constexpr DXGI_FORMAT Format = DXGI_FORMAT_R32G32B32_FLOAT;
		static constexpr const char* SemanticName = "Normal";
	};

	template<>
	struct VertexElement<ElementType::Float3Colour>
	{
		using DataType = DirectX::XMFLOAT3;
		static constexpr DXGI_FORMAT Format = DXGI_FORMAT_R32G32B32_FLOAT;
		static constexpr const char* SemanticName = "Color";
	};

	template<>
	struct VertexElement<ElementType::Float4Colour>
	{
		using DataType = DirectX::XMFLOAT4;
		static constexpr DXGI_FORMAT Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		static constexpr const char* SemanticName = "Color";
	};

	template<>
	struct VertexElement<ElementType::RGBAColour>
	{
		using DataType = RGBAColour;
		static constexpr DXGI_FORMAT Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		static constexpr const char* SemanticName = "Color";
	};

	/* Single element of a vertex */
	class Element
	{
	public:
		Element(ElementType Type, size_t Offset)
			: m_Type(Type), m_Offset(Offset)
		{

		}

		/* Gets the size offset for this element */
		size_t GetOffset() const;
		/* Gets the size offset for the next element */
		size_t GetOffsetAfter() const;
		size_t GetSize() const;

		ElementType GetType() const;
		/* Translates the element to a DX11 element description */
		D3D11_INPUT_ELEMENT_DESC GetDescription() const;

	private:
		static constexpr size_t GetSizeOf(ElementType Type);

		template<ElementType type>
		static constexpr D3D11_INPUT_ELEMENT_DESC GenerateDescription(size_t offset) noexcept
		{
			return { VertexElement<type>::SemanticName,0,VertexElement<type>::Format,0,(UINT)offset,D3D11_INPUT_PER_VERTEX_DATA,0 };
		}

	private:
		ElementType m_Type;
		size_t m_Offset;
	};
}