#pragma once

//= Includes ==============================================================

#include "Flow/Rendering/Core/Bindable.h"
#include "Flow/Rendering/Core/Bindables/ConstantBuffers/DynamicConstantBuffer.h"

//= Class Defintion =======================================================

/* Base class for dynamic pixel CB, use cached or non-cached version at runtime.*/
class PixelConstantBufferDynamic : public Bindable
{
public:

	//= Public Functions =========================================================================

	void											Update(const DynamicCB::Buffer& Buffer);
	void											Bind() override;
	virtual const DynamicCB::LayoutElement&			GetRootLayoutElement() const = 0;

protected:

	//= Protected Functions ======================================================================

													PixelConstantBufferDynamic(const DynamicCB::LayoutElement& LayoutRoot, UINT Slot, const DynamicCB::Buffer* Buffer);

private:

	//= Private Variables ========================================================================

	Microsoft::WRL::ComPtr<ID3D11Buffer>			m_ConstantBuffer;
	UINT											m_Slot;
};

class CachedPixelConstantBufferDynamic : public PixelConstantBufferDynamic
{
public:

	//= Public Functions =========================================================================

													CachedPixelConstantBufferDynamic(const DynamicCB::CookedLayout& Layout, UINT Slot);
													CachedPixelConstantBufferDynamic(const DynamicCB::Buffer& Buffer, UINT Slot);

	const DynamicCB::LayoutElement&					GetRootLayoutElement() const override;
	const DynamicCB::Buffer&						GetBuffer() const;
	void											SetBuffer(const DynamicCB::Buffer& Buffer);

	void											AcceptProbe(TechniqueProbe& Probe) override;
	void											Bind() override;

private:

	//= Private Variables ========================================================================

	bool											m_Dirty = false;
	DynamicCB::Buffer								m_Buffer;
};

class NoCachePixelConstantBufferDynamic : public PixelConstantBufferDynamic
{
	//= Private Functions ========================================================================

													NoCachePixelConstantBufferDynamic(const DynamicCB::CookedLayout& Layout, UINT Slot);
													NoCachePixelConstantBufferDynamic(const DynamicCB::Buffer& Buffer, UINT Slot);
	const DynamicCB::LayoutElement&					GetRootLayoutElement() const override;

private:

	//= Private Variables ========================================================================
	std::shared_ptr<DynamicCB::LayoutElement>		m_LayoutRoot;	
};