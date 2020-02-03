#pragma once
#include "VertexLayout.h"
#include "Vertex.h"

namespace Flow
{
	class VertexBuffer
	{
	public:
		VertexBuffer(VertexLayout& Layout)
			: m_Layout(Layout)
		{

		}

		const VertexLayout& GetLayout() const;
		size_t GetBufferSize() const;
		size_t GetBufferSizeAsBytes() const;
		const char* GetData() const;

		Vertex GetBack();  //Not const so we can use the non-const vector::data()
		Vertex GetFront();

		Vertex operator[](size_t i);

		template<typename ...Params>
		void EmplaceBack(Params&&... params)
		{
			assert(sizeof...(params) == m_Layout.GetElementCount() && "Param count doesn't match number of vertex elements");

			m_Buffer.resize(m_Buffer.size() + m_Layout.GetSize());
			GetBack().SetAttributeByIndex(0u, std::forward<Params>(params)...);
		}

	private:
		VertexLayout m_Layout;
		std::vector<char> m_Buffer;
	};
}