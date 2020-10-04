#include "Flowpch.h"
#include "DynamicShaderBuffers.h"
#include "Flow/Rendering/Core/TechniqueProbe.h"

PixelConstantBufferDynamic::PixelConstantBufferDynamic(const DynamicCB::LayoutElement& LayoutRoot, UINT Slot, const DynamicCB::Buffer* Buffer)
{
	CREATE_RESULT_HANDLE();

	D3D11_BUFFER_DESC ConstantBufferDescription;
	ConstantBufferDescription.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	ConstantBufferDescription.Usage = D3D11_USAGE_DYNAMIC;
	ConstantBufferDescription.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	ConstantBufferDescription.MiscFlags = 0u;
	ConstantBufferDescription.ByteWidth = (UINT)LayoutRoot.GetSizeInBytes();
	ConstantBufferDescription.StructureByteStride = 0u;

	if (Buffer)
	{
		D3D11_SUBRESOURCE_DATA CSD = {};
		CSD.pSysMem = Buffer->GetData();
		CATCH_ERROR_DX(RenderCommand::DX11GetDevice()->CreateBuffer(&ConstantBufferDescription, &CSD, &m_ConstantBuffer));
	}
	else
		CATCH_ERROR_DX(RenderCommand::DX11GetDevice()->CreateBuffer(&ConstantBufferDescription, nullptr, &m_ConstantBuffer));
}

void PixelConstantBufferDynamic::Update(const DynamicCB::Buffer& Buffer)
{
	assert(&Buffer.GetRootLayoutElement() == &GetRootLayoutElement());

	CREATE_RESULT_HANDLE();

	D3D11_MAPPED_SUBRESOURCE MSR;
	CATCH_ERROR_DX(RenderCommand::DX11GetContext()->Map(
		m_ConstantBuffer.Get(), 0u, D3D11_MAP_WRITE_DISCARD, 0u, &MSR));

	memcpy(MSR.pData, Buffer.GetData(), Buffer.GetSizeInBytes());
}

void PixelConstantBufferDynamic::Bind()
{

}

//= Cached PCBD ==============

CachedPixelConstantBufferDynamic::CachedPixelConstantBufferDynamic(const DynamicCB::CookedLayout& Layout, UINT Slot)
	: PixelConstantBufferDynamic(*Layout.ShareRoot(), Slot, nullptr), m_Buffer(DynamicCB::Buffer(Layout))
{
}

CachedPixelConstantBufferDynamic::CachedPixelConstantBufferDynamic(const DynamicCB::Buffer& Buffer, UINT Slot)
	: PixelConstantBufferDynamic(Buffer.GetRootLayoutElement(), Slot, nullptr), m_Buffer(Buffer)
{
}

const DynamicCB::LayoutElement& CachedPixelConstantBufferDynamic::GetRootLayoutElement() const
{
	return m_Buffer.GetRootLayoutElement();
}

const DynamicCB::Buffer& CachedPixelConstantBufferDynamic::GetBuffer() const
{
	return m_Buffer;
}

void CachedPixelConstantBufferDynamic::SetBuffer(const DynamicCB::Buffer& Buffer)
{
	m_Buffer.CopyFrom(Buffer);
	m_Dirty = true;
}

void CachedPixelConstantBufferDynamic::AcceptProbe(TechniqueProbe& Probe)
{
	if (Probe.VisitBuffer(m_Buffer))
		m_Dirty = true;
}

void CachedPixelConstantBufferDynamic::Bind()
{
	if (m_Dirty)
	{
		Update(m_Buffer);
		m_Dirty = false;
	}

	PixelConstantBufferDynamic::Bind();
}

//= No Cache PCBD =========

NoCachePixelConstantBufferDynamic::NoCachePixelConstantBufferDynamic(const DynamicCB::CookedLayout& Layout, UINT Slot)
	: PixelConstantBufferDynamic(*Layout.ShareRoot(), Slot, nullptr), m_LayoutRoot(Layout.ShareRoot())
{
}

NoCachePixelConstantBufferDynamic::NoCachePixelConstantBufferDynamic(const DynamicCB::Buffer& Buffer, UINT Slot)
	: PixelConstantBufferDynamic(Buffer.GetRootLayoutElement(), Slot, nullptr), m_LayoutRoot(Buffer.ShareLayoutRoot())
{
}

const DynamicCB::LayoutElement& NoCachePixelConstantBufferDynamic::GetRootLayoutElement() const
{
	return *m_LayoutRoot;
}
