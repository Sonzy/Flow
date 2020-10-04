#pragma once

//= Includes ============================================

#include "Rendering/Core/Bindables/ConstantBuffers/TransformConstantBuffer.h"

//= Class Definition ====================================

class ScaledTransformConstantBuffer : public TransformConstantBuffer
{
public:

	//= Public Functions ==========================================================

						ScaledTransformConstantBuffer(Renderable* Parent, UINT VertexSlot = 0);
	virtual void		Bind() override;
};