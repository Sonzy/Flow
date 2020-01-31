#include "Flowpch.h"
#include "BindableVertexBuffer.h"
#include "Flow/ErrorHandling/ErrorMacros.h"

namespace Flow
{
	BindableVertexBuffer::BindableVertexBuffer(const VertexBuffer& Buffer)
		: m_Stride((UINT)Buffer.GetLayout().GetSize())
	{
		HRESULT ResultHandle;

		D3D11_BUFFER_DESC bufferDesc;
		bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		bufferDesc.CPUAccessFlags = 0u;
		bufferDesc.MiscFlags = 0u;
		bufferDesc.ByteWidth = UINT(Buffer.GetBufferSizeAsBytes());
		bufferDesc.StructureByteStride = m_Stride;

		D3D11_SUBRESOURCE_DATA subData = {};
		subData.pSysMem = Buffer.GetData();

		//TODO: Get Device
		//CATCH_ERROR_DX(GetDevice(gfx)->CreateBuffer(&bufferDesc, &subData, &vertexBuffer));
	}

	void BindableVertexBuffer::Bind()
	{
	}
}