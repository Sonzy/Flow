#pragma once
#include "Flow/Rendering/Core/Bindables/ConstantBuffers/TransformConstantBuffer.h"

class ScaledTransformConstantBuffer : public TransformConstantBuffer
{
public:
	ScaledTransformConstantBuffer(Renderable* Parent, UINT VertexSlot = 0);

	virtual void Bind() override;
};