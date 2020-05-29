#include "Flowpch.h"
#include "IndexBuffer.h"
#include "BindableCodex.h"

IndexBuffer::IndexBuffer(const std::string& Tag, const std::vector<unsigned short>& Indices)
	: _Count((UINT)Indices.size()), _Tag(Tag)
{
	CREATE_RESULT_HANDLE();

	//Bind the index buffer
	D3D11_BUFFER_DESC IndexBufferDescription = {};
	IndexBufferDescription.BindFlags = D3D11_BIND_INDEX_BUFFER;
	IndexBufferDescription.Usage = D3D11_USAGE_DEFAULT;
	IndexBufferDescription.CPUAccessFlags = 0u;
	IndexBufferDescription.MiscFlags = 0u;
	IndexBufferDescription.ByteWidth = UINT(_Count * sizeof(unsigned short));
	IndexBufferDescription.StructureByteStride = sizeof(unsigned short);

	D3D11_SUBRESOURCE_DATA SubresourceData = {};
	SubresourceData.pSysMem = Indices.data();

	CATCH_ERROR_DX(RenderCommand::DX11GetDevice()->CreateBuffer(&IndexBufferDescription, &SubresourceData, &_IndexBuffer));
}

void IndexBuffer::Bind()
{
	RenderCommand::DX11GetContext()->IASetIndexBuffer(_IndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);
}

UINT IndexBuffer::GetCount() const
{
	return _Count;
}

std::shared_ptr<Bindable> IndexBuffer::Resolve(const std::string& Tag, const std::vector<unsigned short>& Indices)
{
	return BindableCodex::Resolve<IndexBuffer>(Tag, Indices);
}

std::string IndexBuffer::GetUID() const
{
	return GenerateUID(_Tag);
}

std::string IndexBuffer::GenerateUID_Internal(const std::string& Tag)
{
	using namespace std::string_literals;
	return typeid(IndexBuffer).name() + "#"s + Tag;
}


