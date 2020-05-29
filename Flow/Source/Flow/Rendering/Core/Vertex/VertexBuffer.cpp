#include "Flowpch.h"
#include "VertexBuffer.h"
#include "Flow/Rendering/Renderer.h"
#include "Flow/Window/Window.h"

const VertexLayout& VertexBuffer::GetLayout() const
{
	return VertexLayout_;
}

size_t VertexBuffer::GetBufferSize() const
{
	return Buffer_.size() / VertexLayout_.GetSize();
}

size_t VertexBuffer::GetBufferSizeAsBytes() const
{
	return Buffer_.size();
}

const char* VertexBuffer::GetData() const
{
	return Buffer_.data();
}

Vertex VertexBuffer::GetBack()
{
	assert(Buffer_.size() != 0u);
	return Vertex{ Buffer_.data() + Buffer_.size() - VertexLayout_.GetSize(), VertexLayout_ };
}

Vertex VertexBuffer::GetFront()
{
	assert(Buffer_.size() != 0u);
	return Vertex{ Buffer_.data(), VertexLayout_ };
}

Vertex VertexBuffer::operator[](size_t i)
{
	assert(i < GetBufferSize());
	return Vertex{ Buffer_.data() + VertexLayout_.GetSize() * i, VertexLayout_ };
}