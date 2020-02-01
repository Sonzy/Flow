#include "Flowpch.h"
#include "IndexBuffer.h"

Flow::IndexBuffer::IndexBuffer(const std::vector<unsigned short>& Indices)
	: m_Count((UINT)Indices.size())
{
	CREATE_RESULT_HANDLE();

	//Bind the index buffer
	D3D11_BUFFER_DESC IndexBufferDescription = {};
	IndexBufferDescription.BindFlags = D3D11_BIND_INDEX_BUFFER;
	IndexBufferDescription.Usage = D3D11_USAGE_DEFAULT;
	IndexBufferDescription.CPUAccessFlags = 0u;
	IndexBufferDescription.MiscFlags = 0u;
	IndexBufferDescription.ByteWidth = UINT(m_Count * sizeof(unsigned short));
	IndexBufferDescription.StructureByteStride = sizeof(unsigned short);

	D3D11_SUBRESOURCE_DATA SubresourceData = {};
	SubresourceData.pSysMem = Indices.data();

	CATCH_ERROR_DX(RenderCommand::DX11GetDevice()->CreateBuffer(&IndexBufferDescription, &SubresourceData, &m_IndexBuffer));
}

void Flow::IndexBuffer::Bind()
{
	RenderCommand::DX11GetContext()->IASetIndexBuffer(m_IndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);
}

UINT Flow::IndexBuffer::GetCount() const
{
	return m_Count;
}
