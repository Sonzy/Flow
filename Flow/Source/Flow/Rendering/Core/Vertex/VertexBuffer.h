#pragma once

//= Includes =======================

#include "VertexLayout.h"
#include "Vertex.h"

//= Class Definition ===============

class VertexBuffer
{
public:

	//= Public Functions ===========================

	VertexBuffer(VertexLayout& Layout)
		: m_VertexLayout(Layout)
	{}

	const VertexLayout&				GetLayout() const;
	size_t							GetBufferSize() const;
	size_t							GetBufferSizeAsBytes() const;
	const char*						GetData() const;
	Vertex							GetBack();  //Not const so we can use the non-const vector::data()
	Vertex							GetFront();

	Vertex operator[](size_t i);

	//= Public Template Functions ==================

	template<typename ...Params>
	void EmplaceBack(Params&&... params)
	{
		assert(sizeof...(params) == m_VertexLayout.GetElementCount() && "Param count doesn't match number of vertex elements");

		m_Buffer.resize(m_Buffer.size() + m_VertexLayout.GetSize());
		GetBack().SetAttributeByIndex(0u, std::forward<Params>(params)...);
	}

private:
	VertexLayout					m_VertexLayout;
	std::vector<char>				m_Buffer;
};