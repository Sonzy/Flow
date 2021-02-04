//= Includes ==========================================

#include "Flowpch.h"
#include "BindableVertexBuffer.h"
#include "Flow/ErrorHandling/ErrorMacros.h"
#include "BindableCodex.h"

//= Class (BindableVertexBuffer) Definition ===========

BindableVertexBuffer::BindableVertexBuffer(const std::string& Tag, const VertexBuffer& Buffer)
	: m_Stride((UINT)Buffer.GetLayout().GetSize())
	, m_Tag(Tag)
	, m_layout(Buffer.GetLayout())
{
	CREATE_RESULT_HANDLE();

	D3D11_BUFFER_DESC BufferDescription;
	BufferDescription.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	BufferDescription.Usage = D3D11_USAGE_DEFAULT;
	BufferDescription.CPUAccessFlags = 0u;
	BufferDescription.MiscFlags = 0u;
	BufferDescription.ByteWidth = UINT(Buffer.GetBufferSizeAsBytes());
	BufferDescription.StructureByteStride = m_Stride;

	D3D11_SUBRESOURCE_DATA SubresourceData = {};
	SubresourceData.pSysMem = Buffer.GetData();

	CATCH_ERROR_DX(RenderCommand::DX11GetDevice()->CreateBuffer(&BufferDescription, &SubresourceData, &m_Buffer));

#if WITH_EDITOR
	m_d3dlayout = Buffer.GetLayout().GetD3DLayout();
#endif
}

void BindableVertexBuffer::Bind()
{
	const UINT offset = 0u;
	RenderCommand::DX11GetContext()->IASetVertexBuffers(0u, 1u, m_Buffer.GetAddressOf(), &m_Stride, &offset);
}
Bindable* BindableVertexBuffer::Resolve(const std::string& Tag, const VertexBuffer& Buffer)
{
	return BindableCodex::Resolve<BindableVertexBuffer>(Tag, Buffer);
}

std::string BindableVertexBuffer::GetUID() const
{
	return GenerateUID(m_Tag);
}
const VertexLayout& BindableVertexBuffer::GetLayout() const
{
	return m_layout;
}

std::string BindableVertexBuffer::GenerateUID_Internal(const std::string& Tag)
{
	using namespace std::string_literals;
	return typeid(BindableVertexBuffer).name() + "#"s + Tag;
}