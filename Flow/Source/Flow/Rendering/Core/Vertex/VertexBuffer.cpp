//= Includes ===========================

#include "pch.h"
#include "VertexBuffer.h"
#include "Rendering/Renderer.h"
#include "Window/Window.h"

//= Class Definition ===================

const VertexLayout& VertexBuffer::GetLayout() const
{
	return m_VertexLayout;
}

size_t VertexBuffer::GetBufferSize() const
{
	return m_Buffer.size() / m_VertexLayout.GetSize();
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
	return Vertex{ m_Buffer.data() + m_Buffer.size() - m_VertexLayout.GetSize(), m_VertexLayout };
}

Vertex VertexBuffer::GetFront()
{
	assert(m_Buffer.size() != 0u);
	return Vertex{ m_Buffer.data(), m_VertexLayout };
}

Vertex VertexBuffer::operator[](size_t i)
{
	assert(i < GetBufferSize());
	return Vertex{ m_Buffer.data() + m_VertexLayout.GetSize() * i, m_VertexLayout };
}