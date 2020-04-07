#include "Flowpch.h"
#include "BindableVertexBuffer.h"
#include "Flow/ErrorHandling/ErrorMacros.h"
#include "BindableCodex.h"

namespace Flow
{
	BindableVertexBuffer::BindableVertexBuffer(const std::string& Tag, const VertexBuffer& Buffer)
		: Stride_((UINT)Buffer.GetLayout().GetSize()), Tag_(Tag)
	{
		CREATE_RESULT_HANDLE();

		D3D11_BUFFER_DESC BufferDescription;
		BufferDescription.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		BufferDescription.Usage = D3D11_USAGE_DEFAULT;
		BufferDescription.CPUAccessFlags = 0u;
		BufferDescription.MiscFlags = 0u;
		BufferDescription.ByteWidth = UINT(Buffer.GetBufferSizeAsBytes());
		BufferDescription.StructureByteStride = Stride_;

		D3D11_SUBRESOURCE_DATA SubresourceData = {};
		SubresourceData.pSysMem = Buffer.GetData();

		CATCH_ERROR_DX(RenderCommand::DX11GetDevice()->CreateBuffer(&BufferDescription, &SubresourceData, &Buffer_));
	}

	void BindableVertexBuffer::Bind()
	{
		const UINT offset = 0u;
		RenderCommand::DX11GetContext()->IASetVertexBuffers(0u, 1u, Buffer_.GetAddressOf(), &Stride_, &offset);
	}
	std::shared_ptr<Bindable> BindableVertexBuffer::Resolve(const std::string& Tag, const VertexBuffer& Buffer)
	{
		return BindableCodex::Resolve<BindableVertexBuffer>(Tag, Buffer);
	}

	std::string BindableVertexBuffer::GetUID() const
	{
		return GenerateUID(Tag_);
	}
	std::string BindableVertexBuffer::GenerateUID_Internal(const std::string& Tag)
	{
		using namespace std::string_literals;
		return typeid(BindableVertexBuffer).name() + "#"s + Tag;
	}
}