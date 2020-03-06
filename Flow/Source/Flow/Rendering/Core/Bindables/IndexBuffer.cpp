#include "Flowpch.h"
#include "IndexBuffer.h"
#include "BindableCodex.h"

namespace Flow
{
	IndexBuffer::IndexBuffer(const std::string& Tag, const std::vector<unsigned short>& Indices)
		: m_Count((UINT)Indices.size()), m_Tag(Tag)
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

	void IndexBuffer::Bind()
	{
		RenderCommand::DX11GetContext()->IASetIndexBuffer(m_IndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);
	}

	UINT IndexBuffer::GetCount() const
	{
		return m_Count;
	}

	std::shared_ptr<Bindable> IndexBuffer::Resolve(const std::string& Tag, const std::vector<unsigned short>& Indices)
	{
		return BindableCodex::Resolve<IndexBuffer>(Tag, Indices);
	}

	std::string IndexBuffer::GetUID() const
	{
		return GenerateUID(m_Tag);
	}

	std::string IndexBuffer::GenerateUID_Internal(const std::string& Tag)
	{
		using namespace std::string_literals;
		return typeid(IndexBuffer).name() + "#"s + Tag;
	}
}


