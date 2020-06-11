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
		CATCH_ERROR_DX(RenderCommand::DX11GetDevice()->CreateBuffer(&ConstantBufferDescription, &CSD, &_ConstantBuffer));
	}
	else
		CATCH_ERROR_DX(RenderCommand::DX11GetDevice()->CreateBuffer(&ConstantBufferDescription, nullptr, &_ConstantBuffer));
}

void PixelConstantBufferDynamic::Update(const DynamicCB::Buffer& Buffer)
{
	assert(&Buffer.GetRootLayoutElement() == &GetRootLayoutElement());

	CREATE_RESULT_HANDLE();

	D3D11_MAPPED_SUBRESOURCE MSR;
	CATCH_ERROR_DX(RenderCommand::DX11GetContext()->Map(
		_ConstantBuffer.Get(), 0u, D3D11_MAP_WRITE_DISCARD, 0u, &MSR));

	memcpy(MSR.pData, Buffer.GetData(), Buffer.GetSizeInBytes());
}

void PixelConstantBufferDynamic::Bind()
{

}

//= Cached PCBD ==============

CachedPixelConstantBufferDynamic::CachedPixelConstantBufferDynamic(const DynamicCB::CookedLayout& Layout, UINT Slot)
	: PixelConstantBufferDynamic(*Layout.ShareRoot(), Slot, nullptr), _Buffer(DynamicCB::Buffer(Layout))
{
}

CachedPixelConstantBufferDynamic::CachedPixelConstantBufferDynamic(const DynamicCB::Buffer& Buffer, UINT Slot)
	: PixelConstantBufferDynamic(Buffer.GetRootLayoutElement(), Slot, nullptr), _Buffer(Buffer)
{
}

const DynamicCB::LayoutElement& CachedPixelConstantBufferDynamic::GetRootLayoutElement() const
{
	return _Buffer.GetRootLayoutElement();
}

const DynamicCB::Buffer& CachedPixelConstantBufferDynamic::GetBuffer() const
{
	return _Buffer;
}

void CachedPixelConstantBufferDynamic::SetBuffer(const DynamicCB::Buffer& Buffer)
{
	_Buffer.CopyFrom(Buffer);
	_Dirty = true;
}

void CachedPixelConstantBufferDynamic::AcceptProbe(TechniqueProbe& Probe)
{
	if (Probe.VisitBuffer(_Buffer))
		_Dirty = true;
}

void CachedPixelConstantBufferDynamic::Bind()
{
	if (_Dirty)
	{
		Update(_Buffer);
		_Dirty = false;
	}

	PixelConstantBufferDynamic::Bind();
}

//= No Cache PCBD =========

NoCachePixelConstantBufferDynamic::NoCachePixelConstantBufferDynamic(const DynamicCB::CookedLayout& Layout, UINT Slot)
	: PixelConstantBufferDynamic(*Layout.ShareRoot(), Slot, nullptr), _LayoutRoot(Layout.ShareRoot())
{
}

NoCachePixelConstantBufferDynamic::NoCachePixelConstantBufferDynamic(const DynamicCB::Buffer& Buffer, UINT Slot)
	: PixelConstantBufferDynamic(Buffer.GetRootLayoutElement(), Slot, nullptr), _LayoutRoot(Buffer.ShareLayoutRoot())
{
}

const DynamicCB::LayoutElement& NoCachePixelConstantBufferDynamic::GetRootLayoutElement() const
{
	return *_LayoutRoot;
}
