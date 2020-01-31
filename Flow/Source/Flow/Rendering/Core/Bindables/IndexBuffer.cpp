#include "Flowpch.h"
#include "IndexBuffer.h"

Flow::IndexBuffer::IndexBuffer(const std::vector<unsigned short>& Indices)
	: m_Count((UINT)Indices.size())
{
	HRESULT ResultHandle;

	//Bind the index buffer
	D3D11_BUFFER_DESC indexbufferDesc = {};
	indexbufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexbufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexbufferDesc.CPUAccessFlags = 0u;
	indexbufferDesc.MiscFlags = 0u;
	indexbufferDesc.ByteWidth = UINT(m_Count * sizeof(unsigned short));
	indexbufferDesc.StructureByteStride = sizeof(unsigned short);

	D3D11_SUBRESOURCE_DATA indexSubData = {};
	indexSubData.pSysMem = Indices.data();
	//TODO:
	//CATCH_ERROR_DX(GetDevice(gfx)->CreateBuffer(&indexbufferDesc, &indexSubData, &indexBuffer));
}

void Flow::IndexBuffer::Bind()
{
	//TODO
}

UINT Flow::IndexBuffer::GetCount() const
{
	return m_Count;
}
