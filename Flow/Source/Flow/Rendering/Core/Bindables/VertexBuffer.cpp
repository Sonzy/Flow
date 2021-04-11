// Pch /////////////////////////////////////////////////////////

#include "pch.h"

// Includes ////////////////////////////////////////////////////

#include <d3d11.h>
#include "VertexBuffer.h"
#include "Framework/Utils/DirectX11/DirectX11Utils.h"
#include "Rendering/Core/Bindables/Codex.h"
#include "Rendering/Renderer.h"

// Class Definition ////////////////////////////////////////////////////

Bindables::VertexBuffer::VertexBuffer(HashString tag, const ::VertexBuffer& buffer)
	: m_stride((UINT)buffer.GetLayout().GetSize())
	, m_tag(tag)
	, m_layout(buffer.GetLayout())
{
	CreateResultHandle();

	D3D11_BUFFER_DESC BufferDescription;
	BufferDescription.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	BufferDescription.Usage = D3D11_USAGE_DEFAULT;
	BufferDescription.CPUAccessFlags = 0u;
	BufferDescription.MiscFlags = 0u;
	BufferDescription.ByteWidth = UINT(buffer.GetBufferSizeAsBytes());
	BufferDescription.StructureByteStride = m_stride;

	D3D11_SUBRESOURCE_DATA SubresourceData = {};
	SubresourceData.pSysMem = buffer.GetData();

	CaptureDXError(Renderer::GetDevice()->CreateBuffer(&BufferDescription, &SubresourceData, &m_buffer));
}

void Bindables::VertexBuffer::Bind()
{
	const UINT offset = 0u;
	Renderer::GetContext()->IASetVertexBuffers(0u, 1u, m_buffer.GetAddressOf(), &m_stride, &offset);
}

Bindables::VertexBuffer* Bindables::VertexBuffer::Resolve(HashString Tag, const ::VertexBuffer& Buffer)
{
	return Bindables::Codex::Resolve<Bindables::VertexBuffer>(Tag, Buffer);
}

HashString Bindables::VertexBuffer::GetID()
{
	if (m_id.IsNull())
	{
		m_id = GenerateID_Internal(m_tag);
	}
	return m_id;
}

HashString Bindables::VertexBuffer::GenerateID_Internal(HashString tag)
{
	char buffer[64];
#if _DEBUG
	snprintf(buffer, 128, "VertexBuffer-%s", tag.c_str());
#else
	snprintf(buffer, 64, "VertexBuffer-%d", static_cast<int>(type));
#endif
	return buffer;
}