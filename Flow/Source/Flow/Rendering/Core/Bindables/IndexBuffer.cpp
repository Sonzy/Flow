// Includes //////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <d3d11.h>
#include "IndexBuffer.h"
#include "Framework/Utils/DirectX11/DirectX11Utils.h"
#include "Rendering/Core/Bindables/Codex.h"
#include "Rendering/Renderer.h"

// Function Definitions //////////////////////////////////////////////////////////////

Bindables::IndexBuffer::IndexBuffer(HashString tag, const std::vector<uint16>& indices)
	: m_count(indices.size())
	, m_tag(tag)
{
	CreateResultHandle();

	//Bind the index buffer
	D3D11_BUFFER_DESC IndexBufferDescription = {};
	IndexBufferDescription.BindFlags = D3D11_BIND_INDEX_BUFFER;
	IndexBufferDescription.Usage = D3D11_USAGE_DEFAULT;
	IndexBufferDescription.CPUAccessFlags = 0u;
	IndexBufferDescription.MiscFlags = 0u;
	IndexBufferDescription.ByteWidth = UINT(m_count * sizeof(unsigned short));
	IndexBufferDescription.StructureByteStride = sizeof(unsigned short);

	D3D11_SUBRESOURCE_DATA SubresourceData = {};
	SubresourceData.pSysMem = indices.data();

	CaptureDXError(Renderer::GetDevice()->CreateBuffer(&IndexBufferDescription, &SubresourceData, &m_buffer));
}

void Bindables::IndexBuffer::Bind()
{
	Renderer::GetContext()->IASetIndexBuffer(m_buffer.Get(), DXGI_FORMAT_R16_UINT, 0);
}

Bindables::IndexBuffer* Bindables::IndexBuffer::Resolve(HashString tag, const std::vector<uint16>& Indices)
{
	return Bindables::Codex::Resolve<IndexBuffer>(tag, Indices);
}

HashString Bindables::IndexBuffer::GetID()
{
	if (m_id.IsNull())
	{
		m_id = GenerateID(m_tag);
	}
	return m_id;
}

HashString Bindables::IndexBuffer::GenerateID_Internal(HashString tag)
{
	char buffer[64];
#if _DEBUG
	snprintf(buffer, 128, "Topology-%s", tag.c_str());
#else
	snprintf(buffer, 64, "Topology-%d", static_cast<int>(type));
#endif
	return buffer;
}