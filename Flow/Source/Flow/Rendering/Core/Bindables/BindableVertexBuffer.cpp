#include "Flowpch.h"
#include "BindableVertexBuffer.h"
#include "Flow/ErrorHandling/ErrorMacros.h"

namespace Flow
{
	BindableVertexBuffer::BindableVertexBuffer(const VertexBuffer& Buffer)
		: m_Stride((UINT)Buffer.GetLayout().GetSize())
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
	}

	void BindableVertexBuffer::Bind()
	{
		const UINT offset = 0u;
		RenderCommand::DX11GetContext()->IASetVertexBuffers(0u, 1u, m_Buffer.GetAddressOf(), &m_Stride, &offset);
	}
}