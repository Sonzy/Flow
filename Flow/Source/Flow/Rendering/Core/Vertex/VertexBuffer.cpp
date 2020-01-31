#include "Flowpch.h"
#include "VertexBuffer.h"
#include "Flow/Rendering/Renderer.h"
#include "Flow/Window/Window.h"

namespace Flow
{
	const VertexLayout& VertexBuffer::GetLayout() const
	{
		return m_Layout;
	}

	size_t VertexBuffer::GetBufferSize() const
	{
		return m_Buffer.size() / m_Layout.GetSize();
	}

	size_t VertexBuffer::GetBufferSizeAsBytes() const
	{
		return m_Buffer.size();
	}

	const char* VertexBuffer::GetData() const
	{
		return m_Buffer.data();
	}

	Vertex VertexBuffer::GetBack()
	{
		assert(m_Buffer.size() != 0u);
		return Vertex{ m_Buffer.data() + m_Buffer.size() - m_Layout.GetSize(), m_Layout };
	}

	Vertex VertexBuffer::GetFront()
	{
		assert(m_Buffer.size() != 0u);
		return Vertex{ m_Buffer.data(), m_Layout };
	}

	Vertex VertexBuffer::operator[](size_t i)
	{
		assert(i < GetBufferSize());
		return Vertex{ m_Buffer.data() + m_Layout.GetSize() * i, m_Layout };
	}
}


