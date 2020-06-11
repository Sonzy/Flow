#pragma once
#include "Flow/Rendering/Core/Bindable.h"
#include "Flow/Rendering/Core/Bindables/ConstantBuffers/DynamicConstantBuffer.h"

/* Base class for dynamic pixel CB, use cached or non-cached version at runtime.*/
class PixelConstantBufferDynamic : public Bindable
{
protected:

	PixelConstantBufferDynamic(const DynamicCB::LayoutElement& LayoutRoot, UINT Slot, const DynamicCB::Buffer* Buffer);

public:

	void Update(const DynamicCB::Buffer& Buffer);
	void Bind() override;
	virtual const DynamicCB::LayoutElement& GetRootLayoutElement() const = 0;
private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> _ConstantBuffer;
	UINT _Slot;
};

class CachedPixelConstantBufferDynamic : public PixelConstantBufferDynamic
{
public:
	CachedPixelConstantBufferDynamic(const DynamicCB::CookedLayout& Layout, UINT Slot);
	CachedPixelConstantBufferDynamic(const DynamicCB::Buffer& Buffer, UINT Slot);

	const DynamicCB::LayoutElement& GetRootLayoutElement() const override;
	const DynamicCB::Buffer& GetBuffer() const;
	void SetBuffer(const DynamicCB::Buffer& Buffer);

	void AcceptProbe(TechniqueProbe& Probe) override;

	void Bind() override;

private:
	bool _Dirty = false;
	DynamicCB::Buffer _Buffer;
};

class NoCachePixelConstantBufferDynamic : public PixelConstantBufferDynamic
{
	NoCachePixelConstantBufferDynamic(const DynamicCB::CookedLayout& Layout, UINT Slot);
	NoCachePixelConstantBufferDynamic(const DynamicCB::Buffer& Buffer, UINT Slot);

	const DynamicCB::LayoutElement& GetRootLayoutElement() const override;

private:
	std::shared_ptr<DynamicCB::LayoutElement> _LayoutRoot;	
};