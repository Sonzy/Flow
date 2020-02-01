#pragma once
#include "VertexLayout.h"

namespace Flow
{
	class Vertex
	{
		friend class VertexBuffer;
	public:

		/* Used to get a reference to an attribute (e.g Position, Texcoord) */
		template<ElementType Type>
		auto& GetAttribute()
		{
			auto Attribute = Data + m_LayoutRef.GetElement<Type>().GetOffset();//layout.Resolve<Type>().GetOffset();
			return *reinterpret_cast<typename VertexElement<Type>::DataType*>(Attribute);
		}

		/* Sets the value of an attribute by index */
		template<typename T>
		void SetAttributeByIndex(size_t Index, T&& val)
		{
			const auto& Element = m_LayoutRef.GetElementByIndex(Index);
			auto Attribute = Data + Element.GetOffset();

			//TODO: Further research into R/Lvalue forwarding in templates and perfect forwarding
			switch (Element.GetType())
			{
			case VertexLayout::Position2D:
				SetAttribute<ElementType::Position2D>(Attribute, std::forward<T>(val));
				break;
			case VertexLayout::Position3D:
				SetAttribute<ElementType::Position3D>(Attribute, std::forward<T>(val));
				break;
			case VertexLayout::Texture2D:
				SetAttribute<ElementType::Texture2D>(Attribute, std::forward<T>(val));
				break;
			case VertexLayout::Normal:
				SetAttribute<ElementType::Normal>(Attribute, std::forward<T>(val));
				break;
			case VertexLayout::Float3Color:
				SetAttribute<ElementType::Float3Color>(Attribute, std::forward<T>(val));
				break;
			case VertexLayout::Float4Color:
				SetAttribute<ElementType::Float4Color>(Attribute, std::forward<T>(val));
				break;
			case VertexLayout::BGRAColor:
				SetAttribute<ElementType::BGRAColor>(Attribute, std::forward<T>(val));
				break;
			default:
				assert("Bad element type" && false);
			}
		}

	protected:

		Vertex(char* NewData, const VertexLayout& Layout)
			: Data(NewData), m_LayoutRef(Layout)
		{
			assert(NewData != nullptr);
		}

	private:

		template<typename First, typename ...Rest>
		void SetAttributeByIndex(size_t i, First&& first, Rest&&... rest)
		{
			SetAttributeByIndex(i, std::forward<First>(first));
			SetAttributeByIndex(i + 1, std::forward<Rest>(rest)...);
		}

		template<ElementType DestLayoutType, typename SrcType>
		void SetAttribute(char* pAttribute, SrcType&& val) noexcept
		{
			using Dest = typename VertexLayout::VertexElement<DestLayoutType>::DataType;
			if constexpr (std::is_assignable<Dest, SrcType>::value)
				*reinterpret_cast<Dest*>(pAttribute) = val;
			else
				assert("Parameter attribute type mismatch" && false);
		}

	private:
		char* Data = nullptr;
		const VertexLayout& m_LayoutRef;
	};
}